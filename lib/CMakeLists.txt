

# --- lua ----------------------------

file(
    GLOB
    LUA_FILES
    "lua/*.c"
    )

add_library(
    lua
    ${LUA_FILES}
    )

target_include_directories(
    lua
    PUBLIC
    lib/lua
    )

# --- sol2 ---------------------------

add_subdirectory(sol2)

# --- json ---------------------------
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
                cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.tar.gz)
FetchContent_MakeAvailable(json)
