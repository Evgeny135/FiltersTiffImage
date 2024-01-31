#ifndef FILTERSTIFFIMAGE_FILTERSIMAGE_H
#define FILTERSTIFFIMAGE_FILTERSIMAGE_H

#include "Matrix.h"

class FiltersImage {
private:
    Matrix<float> generateGaussKernel(int kernelSize, float sigma);
public:
    FiltersImage() = default;
    Matrix<int> gaussianBlur(Matrix<int> input,const int kernelSize, const float sigma);
    Matrix<int> mediumFilter(const int kernelSize, Matrix<int> input);

    Matrix<int> maximumFilter(int kernelSize, Matrix<int> input);
    static void medianFilterFragment(int kernelSize,Matrix<int> input, Matrix<int> &output, int startRow, int endRow, int startCol, int endCol);
    Matrix<int> minimumFilter(int kernelSize, Matrix<int> input);
};


#endif //FILTERSTIFFIMAGE_FILTERSIMAGE_H
