#include <iostream>
#include "methods.h"

int main() {

    std::cout << "Hello, please enter name of file: ";
    std::string input;
    std::getline(std::cin, input);
    std::cout << std::endl;
    try {
        Methods image(input);

        Methods imageCounterClockwise("input.bmp");

        image.Rotate90();
        image.Save("output_clockwise.bmp");

        imageCounterClockwise.RotateCounter90();
        imageCounterClockwise.Save("output_counterclockwise.bmp");

        int kernelSize = 5;
        std::vector<std::vector<float>> kernel = {
            {1 / 256.0f, 4 / 256.0f, 6 / 256.0f, 4 / 256.0f, 1 / 256.0f},
            {4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f},
            {6 / 256.0f, 24 / 256.0f, 36 / 256.0f, 24 / 256.0f, 6 / 256.0f},
            {4 / 256.0f, 16 / 256.0f, 24 / 256.0f, 16 / 256.0f, 4 / 256.0f},
            {1 / 256.0f, 4 / 256.0f, 6 / 256.0f, 4 / 256.0f, 1 / 256.0f}
        };

        image.GaussianFilter(kernelSize, kernel);
        image.Save("output_gaussian.bmp");

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}