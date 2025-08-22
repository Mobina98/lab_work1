// Include necessary libraries
#include <iostream>    // For console input/output
#include <fstream>     // For file operations
#include <vector>      // For dynamic arrays
#include <memory>      // For smart pointers
#include "bmp.h"       // BMP file structure definitions
#include "methods.h"   // Method declarations
#include <algorithm>   // For algorithms like clamp

// Allocate memory for image data storage
bool Methods::allocateMemory(int height, int width) {
    try {
        // Resize outer vector to hold all image rows
        data.resize(height);
        // Resize each row to hold all image columns
        for (int i = 0; i < height; ++i) {
            data[i].resize(width);
        }
        return true;  // Success
    } catch (const std::bad_alloc&) {
        // Handle memory allocation failure
        std::cerr << "Failed to allocate memory for image data." << std::endl;
        data.clear();  // Clear any partially allocated memory
        return false;  // Failure
    }
}

// Constructor: Load BMP image from file
Methods::Methods(const std::string &filename) {
    // Open file in binary mode
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error opening file.");
    }

    // Read BMP file header
    file.read(reinterpret_cast<char *>(&header), sizeof(header));
    // Check if file is actually a BMP (has "BM" signature)
    if (header.fileType != 0x4D42) {
        throw std::runtime_error("File format is not BMP.");
    }

    // Read BMP info header
    file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

    // Ensure height and width are positive values
    infoHeader.height = std::abs(infoHeader.height);
    infoHeader.width = std::abs(infoHeader.width);

    // Check for valid image dimensions
    if (infoHeader.width == 0 || infoHeader.height == 0) {
        throw std::runtime_error("Unexpected file size.");
    }

    // Jump to the start of pixel data in the file
    file.seekg(header.offsetData, file.beg);

    // Allocate memory for image data
    if (!allocateMemory(infoHeader.height, infoHeader.width)) {
        throw std::runtime_error("Failed to allocate memory for image.");
    }

    // Read pixel data row by row
    for (int i = 0; i < infoHeader.height; ++i) {
        // Read one row of pixels
        file.read(reinterpret_cast<char *>(data[i].data()), infoHeader.width * sizeof(Pixel));
        if (!file) {
            throw std::runtime_error("Error reading file.");
        }
    }

    // Close the file
    file.close();
}

// Destructor: Clean up image data
Methods::~Methods() {
    data.clear();  // Automatically frees memory when container is destroyed
}

// Save image to BMP file
void Methods::Save(const std::string &filename) {
    // Open file for writing in binary mode
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error saving file.");
    }

    // Calculate and display file size information
    int byte = infoHeader.height * infoHeader.width * sizeof(Pixel);
    std::cout << "File " << filename << " uses " << byte << " bytes." << std::endl;

    // Write BMP headers to file
    file.write(reinterpret_cast<const char *>(&header), sizeof(header));
    file.write(reinterpret_cast<const char *>(&infoHeader), sizeof(infoHeader));

    // Write pixel data row by row
    for (int i = 0; i < infoHeader.height; ++i) {
        file.write(reinterpret_cast<const char *>(data[i].data()), infoHeader.width * sizeof(Pixel));
    }

    // Close the file
    file.close();
}

// Rotate image 90 degrees clockwise
void Methods::Rotate90() {
    // Create temporary storage for rotated image (swap width and height)
    std::vector<std::vector<Pixel>> rotatedData(infoHeader.width, std::vector<Pixel>(infoHeader.height));

    // Copy pixels with transformation: (i,j) -> (j, height-i-1)
    for (int i = 0; i < infoHeader.height; ++i) {
        for (int j = 0; j < infoHeader.width; ++j) {
            rotatedData[j][infoHeader.height - i - 1] = data[i][j];
        }
    }

    // Move rotated data to main storage (efficient memory transfer)
    data = std::move(rotatedData);
    // Update image dimensions (swap width and height)
    std::swap(infoHeader.width, infoHeader.height);
}

// Rotate image 90 degrees counter-clockwise
void Methods::RotateCounter90() {
    // Create temporary storage for rotated image (swap width and height)
    std::vector<std::vector<Pixel>> rotatedData(infoHeader.width, std::vector<Pixel>(infoHeader.height));

    // Copy pixels with transformation: (i,j) -> (width-j-1, i)
    for (int i = 0; i < infoHeader.height; ++i) {
        for (int j = 0; j < infoHeader.width; ++j) {
            rotatedData[infoHeader.width - j - 1][i] = data[i][j];
        }
    }

    // Move rotated data to main storage
    data = std::move(rotatedData);
    // Update image dimensions
    std::swap(infoHeader.width, infoHeader.height);
}

// Apply Gaussian blur filter to image
void Methods::GaussianFilter(int kernelSize, const std::vector<std::vector<float>>& kernel) {
    // Validate kernel parameters
    if (kernelSize % 2 == 0 || static_cast<int>(kernel.size()) != kernelSize || static_cast<int>(kernel[0].size()) != kernelSize) {
        throw std::invalid_argument("Kernel size must be odd and match the dimensions of the kernel matrix.");
    }

    // Create temporary buffer for processed pixels
    std::vector<std::vector<Pixel>> tempData(infoHeader.height, std::vector<Pixel>(infoHeader.width));

    // Calculate offset from center of kernel
    int offset = kernelSize / 2;

    // Process each pixel in the image
    for (int y = 0; y < infoHeader.height; ++y) {
        for (int x = 0; x < infoHeader.width; ++x) {
            // Initialize accumulators for each color channel
            float sumRed = 0.0f, sumGreen = 0.0f, sumBlue = 0.0f;

            // Process kernel area around current pixel
            for (int ky = -offset; ky <= offset; ++ky) {
                for (int kx = -offset; kx <= offset; ++kx) {
                    // Calculate neighbor coordinates with boundary checking
                    int nx = std::clamp(x + kx, 0, infoHeader.width - 1);
                    int ny = std::clamp(y + ky, 0, infoHeader.height - 1);

                    // Calculate kernel coordinates
                    int kernelX = kx + offset;
                    int kernelY = ky + offset;

                    // Apply kernel weight to each color channel
                    sumRed   += data[ny][nx].red * kernel[kernelY][kernelX];
                    sumGreen += data[ny][nx].green * kernel[kernelY][kernelX];
                    sumBlue  += data[ny][nx].blue * kernel[kernelY][kernelX];
                }
            }

            // Store results with value clamping to valid color range (0-255)
            tempData[y][x].red   = static_cast<uint8_t>(std::clamp(sumRed, 0.0f, 255.0f));
            tempData[y][x].green = static_cast<uint8_t>(std::clamp(sumGreen, 0.0f, 255.0f));
            tempData[y][x].blue  = static_cast<uint8_t>(std::clamp(sumBlue, 0.0f, 255.0f));
        }
    }

    // Update image data with filtered result
    data = std::move(tempData);
}