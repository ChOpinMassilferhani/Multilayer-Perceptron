add_executable(
    OCR
    main.c
)

set_target_properties(
    OCR
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_link_libraries(
    OCR
    PRIVATE
    ImageProcessing
    NeuralNetwork
    Segmentation
)
