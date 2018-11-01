#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // accepts exactly 3 command line arguments
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // ensures n is positive and < 100
    if (n < 0 || n > 100)
    {
        fprintf(stderr, "n must be a positive integer less than or equal to 100\n");
        return 2;
    }

    // open input file
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    BITMAPINFOHEADER biOld;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }

    // keep original measurement from infile header
    biOld.biWidth = bi.biWidth;
    biOld.biHeight = bi.biHeight;

    // change width and height by n
    bi.biWidth *= n;
    bi.biHeight *= n;

    // determine padding for scanlines (infile)
    int oldPadding = (4 - (biOld.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // padding for scanlines (outfile)
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // change biSizeImage in new header
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);

    // change bfSize in new header
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

   // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(biOld.biHeight); i < biHeight; i++)
    {
        // repeat resized row n times (height)
        for (int j = 0; j < n; j++)
        {
            // iterate over pixels in scanline
            for (int k = 0; k < biOld.biWidth; k++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write to outfile n times
                for (int m = 0; m < n; m++)
                {
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // write outfile's padding
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }

            //send cursor back until n-1 to read a new scanline
            if (j < n-1)
            {
                // send infile cursor back (only iterating RGBTRIPLE)
                fseek(inptr, -(biOld.biWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
            }
        }
            // skip over infile's padding, if any
            fseek(inptr, oldPadding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
