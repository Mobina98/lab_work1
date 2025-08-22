// Prevent including this file multiple times (header guard)
#ifndef BMP_H
#define BMP_H

// Include standard integer types (uint8_t, uint16_t, etc.)
#include <cstdint>

// Set structure packing to 1 byte (no padding between fields)
#pragma pack(push, 1)

// Main BMP file header structure (14 bytes)
struct BMPHeader {
    uint16_t fileType{0x4D42};          // Always "BM" (0x4D42) for BMP files
    uint32_t fileSize{0};               // Total file size in bytes
    uint16_t reserved1{0};              // Reserved (not used)
    uint16_t reserved2{0};              // Reserved (not used)
    uint32_t offsetData{0};             // Offset to start of pixel data
};

// BMP information header (metadata about the image) (40 bytes)
struct BMPInfoHeader {
    uint32_t size{0};                   // Size of this info header (40 bytes)
    int32_t width{0};                   // Image width in pixels
    int32_t height{0};                  // Image height in pixels
    uint16_t planes{1};                 // Number of color planes (must be 1)
    uint16_t bitCount{0};               // Bits per pixel (24 for RGB)
    uint32_t compression{0};            // Compression method (0 = none)
    uint32_t sizeImage{0};              // Size of pixel data in bytes
    int32_t xPixelsPerMeter{0};         // Horizontal resolution (pixels/meter)
    int32_t yPixelsPerMeter{0};         // Vertical resolution (pixels/meter)
    uint32_t colorsUsed{0};             // Number of colors in palette
    uint32_t colorsImportant{0};        // Number of important colors
};

// Restore previous packing alignment
#pragma pack(pop)

// Structure representing a single pixel with RGB color components
struct Pixel {
    uint8_t blue;   // Blue color value (0-255)
    uint8_t green;  // Green color value (0-255)
    uint8_t red;    // Red color value (0-255)
};

// End of header guard
#endif //BMP_H