// Include input/output stream library for console operations
#include <iostream>
// Include custom Methods class for image processing
#include "methods.h"

// Main function - entry point of the program
int main() {

    // Prompt user to enter a filename
    std::cout << "Hello, please enter name of file: ";
    // Variable to store user input
    std::string input;
    // Read the entire line of user input (filename)
    std::getline(std::cin, input);
    // Print a blank line for better formatting
    std::cout << std::endl;
    
    // Try block to catch any exceptions that might occur
    try {
        // Create Methods object by loading the user-specified file
        Methods image(input);

        // Create another Methods object from a fixed filename
        Methods imageCounterClockwise("input.bmp");

        // Rotate the first image 90 degrees clockwise
        image.Rotate90();
        // Save the clockwise rotated image
        image.Save("output_clockwise.bmp");

        // Rotate the second image 90 degrees counter-clockwise
        imageCounterClockwise.RotateCounter90();
        // Save the counter-clockwise rotated image
        imageCounterClockwise.Save("output_counterclockwise.bmp");

        // Set Gaussian filter kernel size to 5x5
        int kernelSize = 5;
        // Define a 5x5 Gaussian blur kernel with specific weights
        std::vector<std::vector<float>> kernel = {
            {1 / 256.0f, 4 / 256.0f, 6 / 256.0f, 4 / 256.0f, 1 / 256.0f},
            {4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f},
            {6 / 256.0f, 24 / 256.0f, 36 / 256.0f, 24 / 256.0f, 6 / 256.0f},
            {4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f},
            {1 / 256.0f, 4 / 256.0f, 6 / 256.0f, 4 / 256.0f, 1 / 256.0f}
        };

        // Apply Gaussian filter to the first image
        image.GaussianFilter(kernelSize, kernel);
        // Save the filtered image
        image.Save("output_gaussian.bmp");

    // Catch any exceptions that occurred in the try block
    } catch (const std::exception &e) {
        // Print the error message to the error output stream
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    // Return 0 indicating successful program execution
    return 0;
}