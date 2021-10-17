cmake_minimum_required (VERSION 3.8)
project ("lib" CXX)

# C++ properties
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library("lib" ${SRC_FILES})
target_include_directories("lib" PUBLIC "${PROJECT_SOURCE_DIR}/include")