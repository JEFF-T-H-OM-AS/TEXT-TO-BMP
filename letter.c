#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define the BMP file header structure
#pragma pack(push, 1) // Ensure no padding between fields
typedef struct {
    uint16_t bfType;           // 2 bytes: Type of the file, 'BM' (0x4D42)
    uint32_t bfSize;           // 4 bytes: Size of the entire file
    uint16_t bfReserved1;      // 2 bytes: Reserved field 1
    uint16_t bfReserved2;      // 2 bytes: Reserved field 2
    uint32_t bfOffBits;        // 4 bytes: Offset to the start of pixel data
} BMPFileHeader;

// Define the Bitmap information header structure (DIB header)
typedef struct {
    uint32_t biSize;           // 4 bytes: Size of this header (40 bytes)
    int32_t biWidth;           // 4 bytes: Width of the image
    int32_t biHeight;          // 4 bytes: Height of the image
    uint16_t biPlanes;         // 2 bytes: Number of color planes (must be 1)
    uint16_t biBitCount;       // 2 bytes: Number of bits per pixel (24 for RGB)
    uint32_t biCompression;    // 4 bytes: Compression type (0 for no compression)
    uint32_t biSizeImage;      // 4 bytes: Size of the image data
    int32_t biXPelsPerMeter;   // 4 bytes: Horizontal resolution (pixels per meter)
    int32_t biYPelsPerMeter;   // 4 bytes: Vertical resolution (pixels per meter)
    uint32_t biClrUsed;        // 4 bytes: Number of colors used (0 for full 24-bit color)
    uint32_t biClrImportant;   // 4 bytes: Number of important colors (0 for all)
} BMPInfoHeader;

// Define the pixel structure (24 bits per pixel)
typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} RGBPixel;

#pragma pack(pop)

// Function to plot a pixel on the image
void plot_pixel(RGBPixel *pixel_data, int width, int x, int y, RGBPixel color) {
    if (x >= 0 && x < width && y >= 0) {
        pixel_data[y * width + x] = color;
    }
}

// Function to create a black BMP image with the letters 'A' and 'B'
void create_bmp_with_text(const char *filename, int width, int height) {
    // File header
    BMPFileHeader fileHeader;
    fileHeader.bfType = 0x4D42;  // 'BM' signature
    fileHeader.bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + width * height * sizeof(RGBPixel);
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader); // Offset to pixel data

    // Info header
    BMPInfoHeader infoHeader;
    infoHeader.biSize = sizeof(BMPInfoHeader);
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;  // 24-bit color depth (RGB)
    infoHeader.biCompression = 0; // No compression
    infoHeader.biSizeImage = width * height * sizeof(RGBPixel);
    infoHeader.biXPelsPerMeter = 2835;  // 72 DPI (standard for BMP)
    infoHeader.biYPelsPerMeter = 2835;  // 72 DPI
    infoHeader.biClrUsed = 0;   // No color palette
    infoHeader.biClrImportant = 0;  // All colors are important

    // Allocate memory for the pixel data (initialized to black)
    RGBPixel *pixel_data = (RGBPixel *)malloc(width * height * sizeof(RGBPixel));
    for (int i = 0; i < width * height; i++) {
        pixel_data[i].red = 0;
        pixel_data[i].green = 0;
        pixel_data[i].blue = 0;
    }

    // Define the color for the letters (white)
    RGBPixel white = {255, 255, 255};

    // Define a 5x7 representation of the letter "A"
    // 0 = empty, 1 = part of the letter 'A'
    int A[7][5] = {
        {0, 0, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1}
    };

    // Define a 5x7 representation of the letter "B"
    // 0 = empty, 1 = part of the letter 'B'
    int B[7][5] = {
        {1, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    // Draw the letter 'A' at position (30, 20)
    int startX_A = 30, startY_A = 20;
    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 5; x++) {
            if (A[6 - y][x] == 1) {  // Flip the rows vertically (y becomes 6 - y)
                plot_pixel(pixel_data, width, startX_A + x, startY_A + y, white);
            }
        }
    }

    // Draw the letter 'B' at position (50, 20)
    int startX_B = 50, startY_B = 20;
    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 5; x++) {
            if (B[6 - y][x] == 1) {  // Flip the rows vertically (y becomes 6 - y)
                plot_pixel(pixel_data, width, startX_B + x, startY_B + y, white);
            }
        }
    }

    // Open the file for writing in binary mode
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
        free(pixel_data);
        return;
    }

    // Write the file header and info header
    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, file);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    // Write the pixel data
    fwrite(pixel_data, sizeof(RGBPixel), width * height, file);

    // Close the file
    fclose(file);

    // Free the allocated memory for pixel data
    free(pixel_data);

    printf("BMP image '%s' with the letters 'A' and 'B' created successfully.\n", filename);
}

int main() {
    int width = 100;  // Image width (in pixels)
    int height = 50;  // Image height (in pixels)

    // Create a black BMP image with the letters "A" and "B"
    create_bmp_with_text("black_image_with_A_and_B.bmp", width, height);

    return 0;
}

