from cs50 import get_float

def main():

    # Prompt user for change owed
    while True:
        change = get_float("Change owed: ")

        # Ensure change is >= 0
        if change >= 0:
            break

    # Rounding off change to cents
    cents = round(change * 100)

    # Initiailize coins
    coins = 0

    # Quarter (0.25$)
    coins += cents // 25
    cents %= 25

    # Dimes (0.10$)
    coins += cents // 10
    cents %= 10

    # Nickels (0.05$)
    coins += cents // 5
    cents %= 5

    # Pennies (0.01$)
    coins += cents // 1
    cents %= 1

    # Print no. of coins used
    print(coins)

if __name__ == "__main__":
    main()



