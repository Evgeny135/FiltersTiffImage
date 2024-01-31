#include "FiltersImage.h"

#include "Matrix.h"
#include "cmath"
#include "algorithm"
#include "iostream"

Matrix<int> copy(Matrix<int> input){
    Matrix<int> output(input.getWidth(),input.getHeight());

    for (int i = 0; i < input.getHeight(); i++) {
        for (int j = 0; j < input.getWidth(); j++) {
            output.set(i,j,input.get(i,j));
        }
    }

    return output;
}


Matrix<float> FiltersImage::generateGaussKernel(int kernelSize, float sigma) {
    if (kernelSize %2 != 0){
        Matrix<float> kernelMatrix(kernelSize,kernelSize);

        float sum = 0.0;
        for (int i = 0; i < kernelSize; ++i) {
            for (int j = 0; j < kernelSize; ++j) {
                int x = i - (kernelSize / 2);
                int y = j - (kernelSize / 2);
                float value = exp(-(x*x + y*y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
                kernelMatrix.set(i,j,value);
                sum += value;
            }
        }

        for (int i = 0; i < kernelSize; ++i) {
            for (int j = 0; j < kernelSize; ++j) {
                kernelMatrix.set(i,j,kernelMatrix.get(i,j)/sum);
            }
        }

        return kernelMatrix;
    }
}

Matrix<int> FiltersImage::gaussianBlur(Matrix<int> input,int kernelSize, float sigma){
    Matrix<float> kernel = generateGaussKernel(kernelSize,sigma);

    Matrix<int> output = copy(input);

    int kCenter = kernel.getWidth() / 2;
    float sum;

    for (int y = 0; y < input.getHeight(); y++) {
        for (int x = 0; x < input.getWidth(); x++) {
            sum = 0;
            for (int k = 0; k < kernel.getWidth(); k++) {
                for (int l = 0; l < kernel.getWidth(); l++) {
                    int imageX = x + l - kCenter;
                    int imageY = y + k - kCenter;

                    if (imageX >= 0 && imageX < input.getWidth() && imageY >= 0 && imageY < input.getHeight()) {
                        sum += input.get(imageY,imageX)*kernel.get(k,l);
                    }
                }
            }
            output.set(y,x,sum);
        }
    }
    return output;
}

Matrix<int> FiltersImage::mediumFilter(int kernelSize, Matrix<int> input) {
    Matrix<int> output = copy(input);

    std::vector<int> neighborhood;

    int halfKernel = kernelSize / 2;

    for (int i = halfKernel; i < input.getHeight() - halfKernel; ++i) {
        for (int j = halfKernel; j < input.getWidth() - halfKernel; ++j) {
            neighborhood.clear();
            for (int k = -halfKernel; k <= halfKernel; ++k) {
                for (int l = -halfKernel; l <= halfKernel; ++l) {
                    neighborhood.push_back(input.get(i+k,j+l));
                }
            }

            int middle = neighborhood.size()/2;

            std::nth_element(neighborhood.begin(),neighborhood.begin()+middle,neighborhood.end());

            output.set(i,j, neighborhood[middle]);
        }
    }
    return output;
}

void FiltersImage::medianFilterFragment(int kernelSize,Matrix<int> input, Matrix<int> &output,
                          int startRow, int endRow, int startCol, int endCol) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = startCol; j < endCol; ++j) {
            std::vector<int> neighborhood;

            int halfFilter = kernelSize/2;

            for (int x = i-halfFilter; x <= i + halfFilter; ++x) {
                for (int y = j - halfFilter; y <= j + halfFilter; ++y) {
                    if (x >= 0 && x < input.getHeight() && y >= 0 && y < input.getWidth()) {
                        neighborhood.push_back(input.get(x,y));
                    }
                }
            }

            int middle = neighborhood.size()/2;

            std::nth_element(neighborhood.begin(),neighborhood.begin()+middle,neighborhood.end());

            output.set(i,j, neighborhood[middle]);
        }
    }
}

Matrix<int> FiltersImage::maximumFilter(int kernelSize, Matrix<int> input){
    Matrix<int> output = copy(input);

    int halfKernel = kernelSize / 2;

    for (int i = halfKernel; i < input.getHeight() - halfKernel; ++i) {
        for (int j = halfKernel; j < input.getWidth() - halfKernel; ++j) {
            int maxValue = std::numeric_limits<int>::min();

            // Находим максимальное значение в окрестности пикселя
            for (int k = -halfKernel; k <= halfKernel; ++k) {
                for (int l = -halfKernel; l <= halfKernel; ++l) {
                    maxValue = std::max(maxValue, input.get(i+k,j+l));
                }
            }

            output.set(i,j,maxValue);
        }
    }
    return output;
}

Matrix<int> FiltersImage::minimumFilter(int kernelSize, Matrix<int> input){
    Matrix<int> output = copy(input);

    int halfKernel = kernelSize / 2;

    for (int i = halfKernel; i < input.getHeight() - halfKernel; ++i) {
        for (int j = halfKernel; j < input.getWidth() - halfKernel; ++j) {
            int minValue = std::numeric_limits<int>::max();

            // Находим максимальное значение в окрестности пикселя
            for (int k = -halfKernel; k <= halfKernel; ++k) {
                for (int l = -halfKernel; l <= halfKernel; ++l) {
                    minValue = std::min(minValue, input.get(i + k, j + l));
                }
            }

            output.set(i, j, minValue);
        }
    }
    return output;
}