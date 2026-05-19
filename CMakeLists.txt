cmake_minimum_required(VERSION 3.25)

project(AnalogComputation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ANALOG_CORE_SOURCES
    src/core/wave.cpp
    src/core/function.cpp
    src/core/transform.cpp
    src/core/interaction.cpp
    src/kernels/kernel.cpp
    src/kernels/archive.cpp
    src/kernels/archive_builder.cpp
)

set(ANALOG_MNIST_SOURCES
    src/encoding/mnist/mnist_sample.cpp
    src/encoding/mnist/mnist_encoder.cpp
    src/encoding/mnist/mnist_loader.cpp
)

add_executable(analog
    src/main.cpp
    ${ANALOG_CORE_SOURCES}
)

add_executable(mnist_archive
    src/mnist_archive_main.cpp
    ${ANALOG_CORE_SOURCES}
    ${ANALOG_MNIST_SOURCES}
)
