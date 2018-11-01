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
    string k = argv[1];

    //Ensures that program is executed with one Command Line Argument
    if (argc != 2)
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }

    //Ensures that Command Line Argument is alphabetical
    for (int i = 0; i < strlen(k); i++)
    {
        if (isalpha(k[i]) == 0)
        {
            printf("Usage: ./vigenere k\n");
            return 1;
        }
    }

    //Declaring a string variable
    plaintext = get_string("plaintext: ");
    printf("ciphertext: ");
    int j = 0;

    //Looping through each character
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {

        //Convert string plaintext into char
        char p = plaintext[i];

        //Check if plaintext is alphabetical
        if (!isalpha(p))
        {
            printf("%c", p);
        }
        else
        {
            //Creating variable m for key (looping)
            int m = j % strlen(k);

            //Check if plaintext is uppercase
            if (isupper(p))
            {
                //Check if letter in k is uppercase
                if (isupper(k[m]))
                {
                    printf("%c", ((((p - UPPER) + (k[m]) - UPPER)) % 26) + UPPER);
                }
                //Check if letter in k is lowercase
                if (islower(k[m]))
                {
                    printf("%c", ((((p - UPPER) + (k[m]) - LOWER)) % 26) + UPPER);
                }
            }

            //Check if plaintext is lowercase
            else if (islower(p))
            {
                //Check if letter in k is uppercase
                if (isupper(k[m]))
                {
                    printf("%c", ((((p - LOWER) + (k[m]) - UPPER)) % 26) + LOWER);
                }
                //Check if letter in k is lowercase
                if (islower(k[m]))
                {
                    printf("%c", ((((p - LOWER) + (k[m]) - LOWER)) % 26) + LOWER);
                }
            }
            j++;
        }
    }
    printf("\n");
    return 0;
}