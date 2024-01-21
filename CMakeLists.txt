cmake_minimum_required(VERSION 3.27)
project(FiltersTiffImage)

find_package(TIFF)

set(CMAKE_CXX_STANDARD 17)

add_executable(FiltersTiffImage main.cpp
        Matrix.h
        FiltersImage.cpp
        FiltersImage.h
        FiltersImage.h
)

target_link_libraries(FiltersTiffImage TIFF::TIFF)