#include <iostream>
#include <tiffio.h>
#include <thread>
#include "Matrix.h"
#include "FiltersImage.h"

int main() {
    TIFF *inputTiff = TIFFOpen("C:\\Users\\trish\\Desktop\\save.tiff", "r");
    if (!inputTiff) {
        std::cerr << "Error: file not found" << std::endl;
        return 1;
    }

    int width, height;
    int bitsPerSample, samplesPerPixel, photoMetric;
    TIFFGetField(inputTiff, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(inputTiff, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(inputTiff, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
    TIFFGetField(inputTiff, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
    TIFFGetField(inputTiff, TIFFTAG_PHOTOMETRIC, &photoMetric);

    TIFF *outputTiff = TIFFOpen("C:\\Users\\trish\\Desktop\\NIIFOTON\\new.tiff", "w");

    TIFFSetField(outputTiff, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(outputTiff, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(outputTiff, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(outputTiff, TIFFTAG_SAMPLESPERPIXEL, samplesPerPixel);
    TIFFSetField(outputTiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(outputTiff, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

    Matrix<int> matrix(width, height);

    uint8_t *raster = (uint8_t *) _TIFFmalloc(width * height * sizeof(uint8_t));

    if (raster) {
        for (int row = 0; row < height; row++) {
            TIFFReadScanline(inputTiff, raster, row, 0);
            for (int col = 0; col < width; col++) {
                uint8_t sample = raster[col];
                matrix.set(row, col, sample);
            }
        }
        _TIFFfree(raster);
    }

    int numThreads = 8;

    Matrix<int> output(matrix.getWidth(),matrix.getHeight());

    for (int i = 0; i < matrix.getHeight(); i++) {
        for (int j = 0; j < matrix.getWidth(); j++) {
            output.set(i,j,matrix.get(i,j));
        }
    }

    int chunkSizeRows = matrix.getHeight() / numThreads;

    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * chunkSizeRows;
        int endRow = (i == numThreads - 1) ? matrix.getHeight() : (i + 1) * chunkSizeRows;

        threads.emplace_back(FiltersImage::medianFilterFragment, 15,std::ref(matrix), std::ref(output),
                             startRow, endRow, 0, matrix.getWidth());
    }

    for (auto& thread : threads) {
        thread.join();
    }

    uint8_t *scanline = (uint8_t *) _TIFFmalloc(width * sizeof(uint8_t));

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            scanline[col] = output.get(row, col);
        }

        if (TIFFWriteScanline(outputTiff, scanline, row) < 0) {
            break;
        }
    }

    TIFFClose(inputTiff);
    TIFFClose(outputTiff);

}
