import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Return result from db
    portfolio = db.execute(
        "SELECT symbol, SUM(quantity) as total_shares, price FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING total_shares > 0 ORDER BY symbol ASC", user_id=session["user_id"])

    # Shows availble cash in db
    rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])

    # Available cash
    cash = rows[0]["cash"]

    # Intializing new variable to add grand total
    grand = cash

    # For each stock in portfolio, assign dictionary keys
    for stock in portfolio:

        # Calling lookup for each stock based on symbol
        quote = lookup(stock["symbol"])

        # Price of each stock
        price = quote["price"]

        # Obtain total shares (quantity) of stock
        shares = int(stock["total_shares"])

        # Determine total value of stock based on quantity * price
        total_value = price * shares

        # Grand total of remaining cash + stocks prices
        grand += total_value

    return render_template("index.html", portfolio=portfolio, cash=cash, grand=grand)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Checking for valid inputs
        # Symbol to purchase stocks
        stock = lookup(request.form.get("symbol"))

        # If symbol does not exist
        if not stock:
            return apology("No stock exists!")

        # Ensures user enters an integer
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Please enter an integer!")

        # Ensure input is a positive integer
        if shares <= 0:
            return apology("Please enter a positive number!")

        # Return cash from users db
        rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])

        # Checks if user can afford stock (tables are 0 based)
        cash = rows[0]["cash"]

        # Return apology if insufficient cash
        if cash < (shares * stock["price"]):
            return apology("Insufficient cash!")

        # Else deduct cash in user table and update stocks in portfolio
        else:
            db.execute("UPDATE users SET cash = cash - :total_price WHERE id = :user_id",
                       user_id=session["user_id"], total_price=(shares * stock["price"]))
            db.execute("INSERT INTO transactions (user_id, symbol, quantity, price) VALUES (:user_id, :symbol, :quantity, :price)",
                       user_id=session["user_id"], symbol=request.form.get("symbol"), quantity=shares, price=stock["price"])

        # Redirect user to home page
        flash("Bought!")
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Obtain transaction history from db
    transactions = db.execute(
        "SELECT symbol, quantity, price, time from transactions where user_id = :user_id", user_id=session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST
    if request.method == "POST":

        # Lookup stock price
        quote = lookup(request.form.get("symbol"))

        # Invalid symbol
        if not quote:
            return apology("No stock exists!")

        # Return stock price
        else:
            return render_template("quoted.html", stock=quote)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username provided
        if not request.form.get("username"):
            return apology("Missing Username!")

        # Ensure password provided
        elif not request.form.get("password"):
            return apology("Missing Password!")

        # Ensure repeated password provided
        elif not request.form.get("confirmation"):
            return apology("Missing Repeated Password!")

        # Ensure password and repeated password matches
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords does not match!")

        # Hashing user's password for database storage (Werkzeug Security Helper)
        hash = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)

        # Check query execution
        results = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                             username=request.form.get("username"), hash=hash)
        if not results:
            # Since username is UNIQUE:
            return apology("Username has already been taken")

        # Log user in upon successful registration
        session["user_id"] = results

        # Redirects user to homepage
        flash("Registered!")
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST
    if request.method == "POST":

        # Checking for valid inputs
        # Symbol to purchase stocks
        stock = lookup(request.form.get("symbol"))

        # If symbol does not exist
        if not stock:
            return apology("No stock exists!")

        # Ensures user enters a valid amount
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Please enter a valid amount!")

        # Ensure input is a positive integer
        if shares <= 0:
            return apology("Please enter a positive number!")

        # Return available shares
        available_shares = db.execute("SELECT SUM(quantity) as total_shares FROM transactions where user_id = :user_id AND symbol = :symbol GROUP BY symbol",
                                      user_id=session["user_id"], symbol=request.form.get("symbol"))

        # Return apology if insufficient cash
        if shares > available_shares[0]["total_shares"]:
            return apology("You do not have enough shares!")

        # Update cash in users db and remove stocks from db
        else:
            db.execute("UPDATE users SET cash = cash + :total_price WHERE id = :user_id",
                       user_id=session["user_id"], total_price=(shares * stock["price"]))
            db.execute("INSERT INTO transactions (user_id, symbol, quantity, price) VALUES (:user_id, :symbol, :quantity, :price)",
                       user_id=session["user_id"], symbol=request.form.get("symbol"), quantity=-shares, price=stock["price"])

        # Redirects user to homepage
        flash("Sold!")
        return redirect("/")

    # User reached route via GET
    else:
        available_shares = db.execute(
            "SELECT symbol, SUM(quantity) as total_shares FROM transactions where user_id = :user_id GROUP BY symbol HAVING total_shares > 0", user_id=session["user_id"])
        return render_template("sell.html", available_shares=available_shares)


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    """Allow user to add additional cash"""

    # User reached route via POST
    if request.method == "POST":

        # Ensures user enters a valid amount
        try:
            amount = float(request.form.get("amount"))
        except:
            return apology("Please enter a valid amount!")

        # Update users database
        db.execute("UPDATE users SET cash = cash + :amount WHERE id = :user_id", user_id=session["user_id"], amount=amount)

        # Redirects user to homepage
        flash("Cash Added!")
        return redirect("/")

    else:
        return render_template("add_cash.html")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
