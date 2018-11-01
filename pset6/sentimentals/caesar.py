import sys
from cs50 import get_string

def main():

    # Ensures only one argument is passed in
    if len(sys.argv) != 2:
        print("Usage: ./caesar k")

    # Converting key to int
    key = int(sys.argv[1])

    print("plaintext: ", end = "")
    plaintext = get_string()

    print("ciphertext: ", end = "")

    # Looping through each character in the plaintext
    for c in plaintext:

        # If alphabetic
        if c.isalpha() == True:

            # If upper case
            if c.isupper() == True:
                # Converting to alphabetical index and back to ASCII
                upper = chr((ord(c) + key - 65) % 26 + 65)
                print(upper, end = "")

            # If lower case
            elif c.islower() == True:
                lower = chr((ord(c) + key - 97) % 26 + 97)
                print(lower, end = "")

        else:
            print(c, end = "")

    # Print newline
    print()

if __name__ == "__main__":
    main()