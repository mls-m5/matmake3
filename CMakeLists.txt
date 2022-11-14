
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


file (
    COPY demos
    DESTINATION .
    )


if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
                cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.tar.gz)
FetchContent_MakeAvailable(json)

target_link_libraries(
    matmake3
    PUBLIC
    nlohmann_json
    )
