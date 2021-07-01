add_library(
    ImageProcessing
    STATIC
    binary.c
    pixel_operations.c
)

target_include_directories(
    ImageProcessing
    PUBLIC
    .
    )

target_link_libraries(
    ImageProcessing
    PUBLIC
    SDL_image
    )

set_target_properties(
    ImageProcessing
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    )

target_compile_definitions(
    ImageProcessing
    PUBLIC
    "_GNU_SOURCE"
    )
