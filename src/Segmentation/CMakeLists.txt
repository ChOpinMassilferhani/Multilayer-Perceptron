add_library(
    Segmentation
    STATIC
    save_letter.c
    segmentation.c
)

target_include_directories(
    Segmentation
    PUBLIC
    .
    )

target_link_libraries(
    Segmentation
    PRIVATE
    )

set_target_properties(
    Segmentation
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    )

target_compile_definitions(
    Segmentation
    PUBLIC
    "_GNU_SOURCE"
    "`pkg-config --cflags sdl` -MMD"
    "-Wall -Wextra -Werror -std=c99 -g -O3"
    "`pkg-config --libs sdl` -lSDL_image"
    )
