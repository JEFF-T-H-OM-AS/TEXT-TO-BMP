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

void create_black_bmp(const char *filename, int width, int height) {
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

    // Open the file for writing in binary mode
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // Write the file header and info header
    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, file);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, file);

    // Create a black image (all pixels set to 0, 0, 0 - black in RGB)
    RGBPixel blackPixel = {0, 0, 0};  // Black color

    // Write pixel data (black image)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fwrite(&blackPixel, sizeof(RGBPixel), 1, file);
        }
    }

    // Close the file
    fclose(file);

    printf("BMP image '%s' created successfully.\n", filename);
}

int main() {
    int width = 100;  // Image width (in pixels)
    int height = 50;  // Image height (in pixels)

    // Create a black BMP image with the specified dimensions
    create_black_bmp("black_image.bmp", width, height);

    return 0;
}

