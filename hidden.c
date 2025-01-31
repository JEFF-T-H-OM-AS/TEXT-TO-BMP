#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include<string.h>
/**

This program converts a text file into a .BMP image. If the image is opened in a text editor, the original text appears.

Written by Adrian Blanco

**/

// Pads a specified amount of zero bytes to the file
void padZeroBytes(int num, FILE * wfile) {
    for (int i = 0; i < num; ++i) {
        putc(0, wfile);
    }
}

// Writes an int to the file in little endian
void writeLittleEndian(int num, FILE * wfile) {
    putc(num & 0xFF, wfile);
    putc((num >> 8) & 0xFF, wfile);
    putc((num >> 16) & 0xFF, wfile);
    putc((num >> 24) & 0xFF, wfile);
}

int main() {
    char ch, file_name[40];
    int textfilesize, picside, arrayside, bitmapsize;
    bool eof = false;
    FILE *rFILE, *wFILE;
    
    // Get the file name from the user
    printf("Enter the name of file: ");
    fgets(file_name, sizeof(file_name), stdin);
    
    // Remove newline character if present
    file_name[strcspn(file_name, "\n")] = 0;

    rFILE = fopen(file_name, "r");

    if (rFILE == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    
    fseek(rFILE, 0L, SEEK_END); // Seek to the end
    textfilesize = ftell(rFILE); // Count the amount of chars in the file
    fseek(rFILE, 0L, SEEK_SET); // Seek to the start

    printf("The size of the text is: %d bytes\n", textfilesize);

    // Each pixel requires 3 bytes, so we need to calculate the side length.
    // The side length of the square is the ceiling of the square root of the file size divided by 3 bytes per pixel.
    picside = (int) ceil(sqrt(textfilesize) / 3.0);

    // Ensure the side length is a multiple of 4 (to align rows to 4-byte boundaries)
    picside = (picside + 3) & ~3;  // Round up to the next multiple of 4

    arrayside = picside * 3;  // 3 bytes per pixel

    // Calculate the size of the bitmap data (with padding to the nearest 4-byte boundary)
    bitmapsize = arrayside * picside + (arrayside % 4) * picside;

    // The array where we store all text
    char textarray[picside][arrayside];

    // Read the contents of the file into our array
    for (int i = 0; i < picside; ++i) {
        for (int j = 0; j < arrayside; ++j) {
            if (!eof) {
                ch = fgetc(rFILE);
                
                // If end of file, set zero
                if (ch == EOF) {
                    eof = true;
                    textarray[i][j] = 0;
                } else {
                    textarray[i][j] = ch;
                }
            } else {
                textarray[i][j] = 0; // Fill the rest with zero
            }
        }
    }

    // Append ".bmp" to the filename
    for (int i = 0; i < sizeof(file_name) / sizeof(char); ++i) {
        if (file_name[i] == '.') {
            file_name[i + 1] = 'b';
            file_name[i + 2] = 'm';
            file_name[i + 3] = 'p';
            break;
        }
    }

    wFILE = fopen(file_name, "wb");    
    if (wFILE == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // BMP Header

    // ID field ('BM')
    putc((int) 'B', wFILE);
    putc((int) 'M', wFILE);

    // Total size of the bmp file
    int bmpfilesize = 54 + bitmapsize; // 54 bytes of headers
    writeLittleEndian(bmpfilesize, wFILE);

    // Unused application-specific data
    padZeroBytes(4, wFILE);

    // 54 bytes offset where the pixel array (bitmap data) can be found
    putc(54, wFILE);
    padZeroBytes(3, wFILE);

    // DIB Header (40 bytes)

    // Size of the DIB header
    putc(40, wFILE);
    padZeroBytes(3, wFILE);

    // Picture width (little endian)
    writeLittleEndian(picside, wFILE);

    // Picture height (little endian)
    writeLittleEndian(picside, wFILE);

    // Color planes (1)
    putc(1, wFILE);
    putc(0, wFILE);

    // 24 bits per pixel (RGB)
    putc(24, wFILE);
    putc(0, wFILE);

    // No compression
    padZeroBytes(4, wFILE);

    // Size of raw bitmap data
    writeLittleEndian(bitmapsize, wFILE);

    // Horizontal and vertical pixel resolution (arbitrary, we set to zero)
    padZeroBytes(4, wFILE);
    padZeroBytes(4, wFILE);

    // Number of colors in the palette (0 for 24-bit color)
    padZeroBytes(4, wFILE);

    // Number of important colors (0 means all colors are important)
    padZeroBytes(4, wFILE);

    // Bitmap Data (Text as pixel data)
    for (int i = 0; i < picside; ++i) {
        for (int j = 0; j < arrayside; ++j) {
            putc(textarray[i][j], wFILE);
        }

        // Pad each row with zeros to ensure it's a multiple of 4 bytes
        for (int p = 0; p < (arrayside % 4); ++p) {
            putc(0, wFILE);
        }
    }

    fclose(rFILE);
    fclose(wFILE);
    return 0;
}

