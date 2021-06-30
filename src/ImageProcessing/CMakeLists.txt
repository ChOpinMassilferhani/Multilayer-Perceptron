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
    PRIVATE
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
    "`pkg-config --cflags sdl` -MMD"
    "-Wall -Wextra -Werror -std=c99 -g -O3"
    "`pkg-config --libs sdl` -lSDL_image"
    )
