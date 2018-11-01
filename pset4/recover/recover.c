#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])

{
    // accepts exactly 1 command line argument
    if (argc != 2)
    {
        fprintf(stderr,"./recover image\n");
        return 1;
    }

    //open input file
    FILE* file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // declaring variables

    // array to store buffer
    typedef uint8_t BYTE;
    BYTE buffer[512];

    // char array to create filename for new jpeg
    char fileName[8];

    // declaring img to store pointer to image file
    FILE* img = NULL;

    // counter
    int counter = 0;
    int counter2 = 0;

    // variable to keep track of first jpeg
    bool first = false;

    // repeat until end of memory card (< 512 bytes)
    // reading 512 blocks, 1 byte each
    while (fread(buffer, 1, 512, file) == 512)
    {
        // beginning of jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 &&
            buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
            {
                // check if first jpeg

                if (!first)
                    {
                        // making a new jpeg
                        sprintf(fileName, "%03i.jpg", counter);
                        counter++;
                        img = fopen(fileName, "w");
                    }

                else
                    {
                        // close previous file and open new one
                        fclose(img);
                        sprintf(fileName, "%03i.jpg", counter);
                        counter++;
                        img = fopen(fileName, "w");
                    }
            }
            // write 512 bytes at a time if it is an image
            if (img != NULL)
                {
                    fwrite (buffer, 1, 512, img);
                }
    counter2++;
    }
    // close infile
    fclose(file);

    // close outfile
    fclose(img);

    //success
    return 0;
}
