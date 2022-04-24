
cmake_minimum_required(VERSION 3.23)
project(matmake3)

add_executable(
    matmake3
    src/main.cpp
    src/ninja.cpp
    src/filter.cpp
    )

target_compile_features(
    matmake3
    PRIVATE
    cxx_std_17
    )

file (
    COPY demos
    DESTINATION .
    )
