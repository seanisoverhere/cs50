#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user for an input (Positive Number)
    int n;
    do
    {
        n = get_int("Positive Number: ");
    }
    while (n < 0 || n > 23);

    for (int i = 0; i < n; i++)
    {
        // Spaces before pyramid
        for (int j = n - i - 1; j > 0; j--)
        {
            printf(" ");
        }
        // For Left Pyramid
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }
        // For Gap
        {
            printf("  ");
        }
        // For Right Pyramid
        for (int m = 0; m < i + 1; m++)
        {
            printf("#");
        }
        printf("\n");
    }
}
