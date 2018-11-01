#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define UPPER 65
#define LOWER 97

int main(int argc, string argv[])

{
    string plaintext;

    //Ensures that program is executed with one Command Line Argument
    if (argc != 2)
    {
        printf("Usage: ./caesar k\n");
        return 1;
    }

    //Converting string (k) to int
    int k = atoi(argv[1]);

    //Declaring a string variable
    plaintext = get_string("plaintext: ");
    printf("ciphertext: ");

    //Looping through each character
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        //Check if character is an alphabet
        char p = plaintext[i];

        if (isalpha(p))
        {
            if (isupper(p))
            {
                //Converting to alphabetical index (encipher) then back to ASCII (printing)
                printf("%c", (((p + k) - UPPER) % 26) + UPPER);
            }
            else if (islower(p))
            {
                printf("%c", (((p + k) - LOWER) % 26) + LOWER);
            }
        }
        else
        {
            printf("%c", p);
        }
    }
    printf("\n");
    return 0;
}