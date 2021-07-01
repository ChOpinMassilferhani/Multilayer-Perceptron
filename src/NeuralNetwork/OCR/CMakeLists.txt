add_library(
    OCR
    STATIC
    OCR.c
)

target_include_directories(
    OCR
    PUBLIC
    .
    ../Network
    ../Tools
    ../../ImageProcessing
    )

target_link_libraries(
    OCR
    PUBLIC
    )

set_target_properties(
    OCR
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    )

target_compile_definitions(
    OCR
    PUBLIC
    "_GNU_SOURCE"
    )
