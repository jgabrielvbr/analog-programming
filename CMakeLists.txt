cmake_minimum_required(VERSION 3.25)

project(AnalogComputation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ANALOG_CORE_SOURCES
    src/core/wave.cpp
    src/core/function.cpp
    src/core/transform.cpp
    src/interactions/superposition.cpp
    src/interactions/subtraction.cpp
    src/interactions/product.cpp
    src/interactions/correlation.cpp
    src/interactions/convolution.cpp
    src/interactions/modulation.cpp
    src/interactions/mixing.cpp
    src/interactions/difference.cpp
    src/presets/preset.cpp
    src/presets/low_pass.cpp
    src/presets/high_pass.cpp
    src/presets/band_pass.cpp
    src/presets/band_stop.cpp
    src/presets/gaussian.cpp
    src/presets/resonance.cpp
)

add_executable(analog
    src/main.cpp
    ${ANALOG_CORE_SOURCES}
)

add_executable(mnist_train
    src/mnist_main.cpp
    src/network/mnist_loader.cpp
    src/network/resonator_node.cpp
    src/network/resonator_layer.cpp
    src/network/resonator_node_2d.cpp
    src/network/resonator_layer_2d.cpp
    src/network/readout.cpp
    src/network/network.cpp
    ${ANALOG_CORE_SOURCES}
)
