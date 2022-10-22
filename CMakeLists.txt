
cmake_minimum_required(VERSION 3.23)
project(matmake3)

add_subdirectory(lib)

add_executable(
    matmake3
    src/main.cpp
    src/ninja.cpp
    src/filter.cpp
    src/glob.cpp
    src/matmakelua.cpp
    src/createrecursive.cpp
    src/deps.cpp
    src/build.cpp
    src/compilationdatabase.cpp
    )

target_compile_features(
    matmake3
    PRIVATE
    cxx_std_17
    )

target_link_libraries(
    matmake3
    PUBLIC
    lua
    )

file (
    COPY demos
    DESTINATION .
    )


include_directories(
    lib/json/single_include/
    )
