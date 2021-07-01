cmake_minimum_required(VERSION 3.0)
project(Executable VERSION 1.0.0 LANGUAGES C)


if(NEED_COVERAGE)
    add_compile_options(-Wall -Wextra -pedantic -g --coverage -lm)
    add_link_options(--coverage)
else()
    add_compile_options(-Wall -Wextra -pedantic -g -lm -MMD -lm -O3)
endif()

add_subdirectory(src)
add_subdirectory(src/Segmentation)
add_subdirectory(src/NeuralNetwork/Tools)
add_subdirectory(src/NeuralNetwork/Network)
add_subdirectory(src/NeuralNetwork/OCR)
add_subdirectory(src/ImageProcessing)

