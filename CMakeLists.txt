cmake_minimum_required(VERSION 3.25)

project(AnalogComputation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(analog
    src/main.cpp
    src/core/wave.cpp
    src/core/function.cpp
    src/filters/low_pass_filter.cpp
    src/filters/high_pass_filter.cpp
    src/filters/band_pass_filter.cpp
    src/filters/band_stop_filter.cpp
)
