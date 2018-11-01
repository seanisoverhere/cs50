#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    //Declaring variables on char 1 and 3
    double numerator = fraction[0] - '0';
    double denominator = fraction[2] - '0';

    //Formula to get the duration in int
    int duration = (numerator / denominator) * 8;
    return duration;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    double frequency = 440.0;
    int octave = 0;
    char accidental = 0;
    char letter = note[0];

    //If there are no accidentals
    if (strlen(note) == 2)
    {
        octave = note[1] - '0';
    }
    //If there are accidentals
    else if (strlen(note) == 3)
    {
        accidental = note[1];
        octave = note[2] - '0';
    }

    //Implement # (sharps) and b (flats)
    //For #
    if (accidental == '#')
    {
        frequency = frequency * pow(2.0, 1.0 / 12.0);
    }
    //For b
    else if (accidental == 'b')
    {
        frequency = frequency / pow(2.0, 1.0 / 12.0);
    }
    //Implement different octaves for A
    if (octave != 4)
    {
        //Formula to change frequnecy based on diffrerent octaves
        double octave_semitone = (4 - octave) * 12;
        frequency = frequency / pow(2.0, octave_semitone / 12.0);
    }
    //Changing by semitones according to letters
    if (letter == 'B')
    {
        frequency = frequency * pow(2.0, 2.0 / 12.0);
    }
    else if (letter == 'C')
    {
        frequency = frequency / pow(2.0, 9.0 / 12.0);
    }
    else if (letter == 'D')
    {
        frequency = frequency / pow(2.0, 7.0 / 12.0);
    }
    else if (letter == 'E')
    {
        frequency = frequency / pow(2.0, 5.0 / 12.0);
    }
    else if (letter == 'F')
    {
        frequency = frequency / pow(2.0, 4.0 / 12.0);
    }
    else if (letter == 'G')
    {
        frequency = frequency / pow(2.0, 2.0 / 12.0);
    }
    return round(frequency);
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    //Checks if "First String" is blank
    if (s[0] == '\0')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
