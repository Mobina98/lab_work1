// Prevent including this file multiple times (header guard)
#ifndef METHODS_H
#define METHODS_H

// Include BMP file structure definitions
#include "bmp.h"
// Include vector container for storing image data
#include <vector>

// Class for BMP image processing methods
class Methods {
public:
    // Constructor: loads BMP image from specified filename
    Methods(const std::string &filename);
    
    // Destructor: cleans up allocated resources
    ~Methods();

    // Save the current image to a BMP file with specified filename
    void Save(const std::string &filename);
    
    // Rotate the image 90 degrees clockwise
    void Rotate90();
    
    // Rotate the image 90 degrees counter-clockwise
    void RotateCounter90();
    
    // Apply Gaussian blur filter with specified kernel size and matrix
    void GaussianFilter(int kernelSize, const std::vector<std::vector<float>>& kernel);

private:
    // BMP file header information
    BMPHeader header;
    
    // BMP info header (image metadata like width, height, etc.)
    BMPInfoHeader infoHeader;
    
    // 2D vector storing the actual pixel data of the image
    // Outer vector: rows, Inner vector: columns
    std::vector<std::vector<Pixel>> data;

    // Helper method to allocate memory for image data storage
    bool allocateMemory(int height, int width);
};

// End of header guard
#endif // METHODS_H