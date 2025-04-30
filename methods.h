#ifndef METHODS_H
#define METHODS_H
#include "bmp.h"
#include <vector>

class Methods {
public:
    Methods(const std::string &filename);
    ~Methods();

    void Save(const std::string &filename);
    void Rotate90();
    void RotateCounter90();
    void GaussianFilter(int kernelSize, const std::vector<std::vector<float>>& kernel);

private:
    BMPHeader header;
    BMPInfoHeader infoHeader;
    std::vector<std::vector<Pixel>> data;

    bool allocateMemory(int height, int width);
};

#endif // METHODS_H