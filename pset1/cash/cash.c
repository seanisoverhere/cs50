#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)

{
    float change;
    int cents;

    //Ensure validity of change
    do
    {
        change = get_float("Change owed: ");
    }
    while (change < 0);

    // Converting change to cents and rounding off
    change = cents = round(change * 100);

    int i = 0;
    //Quarters (0.25$)
    while (cents >= 25)
    {
        i++;
        cents -= 25;
    }
    //Dimes (0.10$)
    while (cents >= 10)
    {
        i++;
        cents -= 10;
    }
    //Nickels (0.05$)
    while (cents >= 5)
    {
        i++;
        cents -= 5;
    }
    //Pennies (0.01$)
    while (cents >= 1)
    {
        i++;
        cents -= 1;
    }
    printf("Coins used: %i\n", i);
}

