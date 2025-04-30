#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "bmp.h"
#include "methods.h"
#include <algorithm>


bool Methods::allocateMemory(int height, int width) {
    try {
        data.resize(height);
        for (int i = 0; i < height; ++i) {
            data[i].resize(width);
        }
        return true;
    } catch (const std::bad_alloc&) {
        std::cerr << "Failed to allocate memory for image data." << std::endl;
        data.clear();
        return false;
    }
}

Methods::Methods(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error opening file.");
    }

    // Читаем заголовки BMP
    file.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header.fileType != 0x4D42) {
        throw std::runtime_error("File format is not BMP.");
    }

    file.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

    infoHeader.height = std::abs(infoHeader.height);
    infoHeader.width = std::abs(infoHeader.width);

    if (infoHeader.width == 0 || infoHeader.height == 0) {
        throw std::runtime_error("Unexpected file size.");
    }

    // Переходим к данным изображения
    file.seekg(header.offsetData, file.beg);

    // Выделяем память построчно
    if (!allocateMemory(infoHeader.height, infoHeader.width)) {
        throw std::runtime_error("Failed to allocate memory for image.");
    }

    // Считываем данные построчно
    for (int i = 0; i < infoHeader.height; ++i) {
        file.read(reinterpret_cast<char *>(data[i].data()), infoHeader.width * sizeof(Pixel));
        if (!file) {
            throw std::runtime_error("Error reading file.");
        }
    }

    file.close();
}

Methods::~Methods() {
    data.clear();  // Автоматически освобождается память при уничтожении контейнера
}

void Methods::Save(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error saving file.");
    }

    int byte = infoHeader.height * infoHeader.width * sizeof(Pixel);

    std::cout << "File " << filename << " uses " << byte << " bytes." << std::endl;

    file.write(reinterpret_cast<const char *>(&header), sizeof(header));
    file.write(reinterpret_cast<const char *>(&infoHeader), sizeof(infoHeader));

    for (int i = 0; i < infoHeader.height; ++i) {
        file.write(reinterpret_cast<const char *>(data[i].data()), infoHeader.width * sizeof(Pixel));
    }

    file.close();
}

void Methods::Rotate90() {
    std::vector<std::vector<Pixel>> rotatedData(infoHeader.width, std::vector<Pixel>(infoHeader.height));

    for (int i = 0; i < infoHeader.height; ++i) {
        for (int j = 0; j < infoHeader.width; ++j) {
            rotatedData[j][infoHeader.height - i - 1] = data[i][j];
        }
    }

    data = std::move(rotatedData);
    std::swap(infoHeader.width, infoHeader.height);
}

void Methods::RotateCounter90() {
    std::vector<std::vector<Pixel>> rotatedData(infoHeader.width, std::vector<Pixel>(infoHeader.height));

    for (int i = 0; i < infoHeader.height; ++i) {
        for (int j = 0; j < infoHeader.width; ++j) {
            rotatedData[infoHeader.width - j - 1][i] = data[i][j];
        }
    }

    data = std::move(rotatedData);
    std::swap(infoHeader.width, infoHeader.height);
}

void Methods::GaussianFilter(int kernelSize, const std::vector<std::vector<float>>& kernel) {
    // Проверка на корректный размер ядра
    if (kernelSize % 2 == 0 || static_cast<int>(kernel.size()) != kernelSize || static_cast<int>(kernel[0].size()) != kernelSize) {
        throw std::invalid_argument("Kernel size must be odd and match the dimensions of the kernel matrix.");
    }

    // Создаем временный буфер для хранения обработанных пикселей
    std::vector<std::vector<Pixel>> tempData(infoHeader.height, std::vector<Pixel>(infoHeader.width));

    int offset = kernelSize / 2; // Смещение от центра ядра

    for (int y = 0; y < infoHeader.height; ++y) {
        for (int x = 0; x < infoHeader.width; ++x) {
            float sumRed = 0.0f, sumGreen = 0.0f, sumBlue = 0.0f;

            // Проход по ядру свертки
            for (int ky = -offset; ky <= offset; ++ky) {
                for (int kx = -offset; kx <= offset; ++kx) {
                    // Координаты пикселей с учетом границ изображения
                    int nx = std::clamp(x + kx, 0, infoHeader.width - 1);
                    int ny = std::clamp(y + ky, 0, infoHeader.height - 1);

                    // Координаты ядра
                    int kernelX = kx + offset;
                    int kernelY = ky + offset;

                    // Применение ядра
                    sumRed   += data[ny][nx].red * kernel[kernelY][kernelX];
                    sumGreen += data[ny][nx].green * kernel[kernelY][kernelX];
                    sumBlue  += data[ny][nx].blue * kernel[kernelY][kernelX];
                }
            }

            // Запись результатов с учетом допустимого диапазона значений
            tempData[y][x].red   = static_cast<uint8_t>(std::clamp(sumRed, 0.0f, 255.0f));
            tempData[y][x].green = static_cast<uint8_t>(std::clamp(sumGreen, 0.0f, 255.0f));
            tempData[y][x].blue  = static_cast<uint8_t>(std::clamp(sumBlue, 0.0f, 255.0f));
        }
    }

    // Обновляем данные изображения
    data = std::move(tempData);
}