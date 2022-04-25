cmake_minimum_required(VERSION 3.23)
project(matmake3)

file(
    GLOB LUA_SRC
    lua/lapi.c
    lua/lcode.c
    lua/lctype.c
    lua/ldebug.c
    lua/ldo.c
    lua/ldump.c
    lua/lfunc.c
    lua/lgc.c
    lua/llex.c
    lua/lmem.c
    lua/lobject.c
    lua/lopcodes.c
    lua/lparser.c
    lua/lstate.c
    lua/lstring.c
    lua/ltable.c
    lua/ltm.c
    lua/lundump.c
    lua/lvm.c
    lua/lzio.c
    lua/lauxlib.c
    lua/lbaselib.c
    lua/lcorolib.c
    lua/ldblib.c
    lua/liolib.c
    lua/lmathlib.c
    lua/loadlib.c
    lua/loslib.c
    lua/lstrlib.c
    lua/ltablib.c
    lua/lutf8lib.c
    lua/linit.c
    )

file(
    GLOB LUACPP_SRC
    luacpp/Source/*.cpp
    luacpp/Source/Engine/*.cpp
    luacpp/Source/Registry/*.cpp
    )

message("${LUACPP_SRC}")

add_library(
    lua
    SHARED
    ${LUA_SRC}
    )

target_include_directories(
    lua
    PUBLIC
    lua
    )

add_library(
    luacpp
    STATIC
    ${LUACPP_SRC}
    )

target_include_directories(
    luacpp
    PUBLIC
    luacpp/Source
    )

target_link_libraries(
    luacpp
    PUBLIC
    lua
    )

target_compile_features(
    luacpp
    PUBLIC
    cxx_std_17
    )

