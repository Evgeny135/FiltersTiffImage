#ifndef FILTERSTIFFIMAGE_FILTERSIMAGE_H
#define FILTERSTIFFIMAGE_FILTERSIMAGE_H

#include "Matrix.h"

class FiltersImage {
private:
    Matrix<float> generateGaussKernel(int kernelSize, float sigma);
public:
    FiltersImage() = default;
    Matrix<int> gaussianBlur(Matrix<int> input,int kernelSize, float sigma);
    Matrix<int> mediumFilter(int kernelSize, Matrix<int> input);

    Matrix<int> maximumFilter(int kernelSize, Matrix<int> input);

    Matrix<int> minimumFilter(int kernelSize, Matrix<int> input);
};


#endif //FILTERSTIFFIMAGE_FILTERSIMAGE_H
