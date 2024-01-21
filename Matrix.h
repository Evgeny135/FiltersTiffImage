#ifndef FILTERSTIFFIMAGE_MATRIX_H
#define FILTERSTIFFIMAGE_MATRIX_H

#include "vector"

template <typename T>
class Matrix {
private:
    int width =0, height = 0;
    std::vector<T> imageMatrix;
public:
    Matrix();
    Matrix(unsigned int width, unsigned int height);
    void set(unsigned int row,unsigned int column, const T& v);
    const T& get(unsigned int row,unsigned int column) const;
    int getWidth() const;
    int getHeight() const;
};

template<typename T>
Matrix<T>::Matrix() = default;

template<typename T>
Matrix<T>::Matrix(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
    imageMatrix.resize(width*height);
}

template<typename T>
void Matrix<T>::set(unsigned int row, unsigned int column, const T &v) {
    imageMatrix[row*width+column] = v;
}

template<typename T>
const T &Matrix<T>::get(unsigned int row, unsigned int column) const {
    return imageMatrix[row*width+column];
}

template<typename T>
int Matrix<T>::getHeight() const {
    return height;
}

template<typename T>
int Matrix<T>::getWidth() const {
    return width;
}


#endif //FILTERSTIFFIMAGE_MATRIX_H
