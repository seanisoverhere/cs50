from cs50 import get_int

def main():

    # Do-while loop to prompt user for an input (Positive Number)
    while True:
        height = get_int("Positive Number: ")

        # Ensures input is a positive number
        if height >= 0 and height <= 23:
            break

    for rows in range(0, height):

        # Spaces before pyramid
        for spaces in range(1, height - rows):
            print(" ", end ="")

        # For left pyramid
        for left_hashses in range(height - rows, height + 1):
            print("#", end ="")

        # For gap
        print ("  ", end ="")

        # For right pyramid
        for right_hashes in range(height - rows, height + 1):
            print ("#", end ="")

        # Print newline
        print()

if __name__ == "__main__":
    main()