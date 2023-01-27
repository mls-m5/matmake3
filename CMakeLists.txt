
cmake_minimum_required(VERSION 3.23)
project(matmake3)

add_subdirectory(lib)

add_executable(
    matmake3
    src/main.cpp
    src/ninja.cpp
    src/filter.cpp
    src/glob.cpp
    src/createrecursive.cpp
    src/deps.cpp
    src/build.cpp
    src/compilationdatabase.cpp
    src/luascript.cpp
    src/index.cpp
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

target_include_directories(
    lua
    PUBLIC
    lib/lua
    )

target_link_libraries(
    matmake3
    PUBLIC
    nlohmann_json
    sol2
    lua
    )
