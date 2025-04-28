#ifndef MAIN_h
#define MAIN_h

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cstdint>

#endif

#pragma pack(push, 1)

// File header for BMP format
struct BITMAPFILEHEADER {
    uint16_t bfType;        // File type (should be 0x4D42 for BMP)
    uint32_t bfSize;        // Size of the file
    uint16_t bfReserved1;   // Reserved, must be 0
    uint16_t bfReserved2;   // Reserved, must be 0
    uint32_t bfOffBits;     // Offset from the start of the file to the pixel data
};

// Info header for BMP format
struct BITMAPINFOHEADER {
    uint32_t biSize;        // Size of this header (40 bytes)
    int32_t biWidth;        // Image width
    int32_t biHeight;       // Image height (negative for top-down BMP)
    uint16_t biPlanes;      // Number of color planes (must be 1)
    uint16_t biBitCount;    // Number of bits per pixel (typically 24)
    uint32_t biCompression; // Compression type (0 for uncompressed)
    uint32_t biSizeImage;   // Size of the image data (including padding)
    int32_t biXPelsPerMeter;  // Horizontal resolution (pixels per meter)
    int32_t biYPelsPerMeter;  // Vertical resolution (pixels per meter)
    uint32_t biClrUsed;     // Number of colors used (0 for default)
    uint32_t biClrImportant; // Number of important colors (0 for all)
};

#pragma pack(pop)