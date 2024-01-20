#include <iostream>
#include <tiffio.h>


int main() {
    TIFF *inputTiff = TIFFOpen("C:\\Users\\trish\\Desktop\\lenna.tif", "r");
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
    TIFFSetField(outputTiff, TIFFTAG_PHOTOMETRIC, photoMetric);
    TIFFSetField(outputTiff, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

    uint16_t *scanlineData = (uint16_t *) _TIFFmalloc(TIFFScanlineSize(inputTiff));

    for (uint32_t row = 0; row < height; row++) {
        TIFFReadScanline(inputTiff, scanlineData, row);

        uint16_t *outputScanlineData = new uint16_t[width * samplesPerPixel];
        for (uint32_t col = 0; col < width; col++) {
            for (uint16_t channel = 0; channel < samplesPerPixel; channel++) {
                uint16_t sample = scanlineData[col * samplesPerPixel + channel];
                outputScanlineData[col * samplesPerPixel + channel] = static_cast<uint16_t>(sample);

            }
        }

        TIFFWriteScanline(outputTiff, outputScanlineData, row);

        delete[] outputScanlineData;
    }

    _TIFFfree(scanlineData);
    TIFFClose(inputTiff);
    TIFFClose(outputTiff);

    return 0;
}
