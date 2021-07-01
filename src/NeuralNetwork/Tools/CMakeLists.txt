add_library(
    Tools
    STATIC
    image_matrice.c
    matrice.c
)

target_include_directories(
    Tools
    PUBLIC
    .
    )

target_link_libraries(
    Tools
    PUBLIC
    ImageProcessing
    )

set_target_properties(
    Tools
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    )

target_compile_definitions(
    Tools
    PUBLIC
    "_GNU_SOURCE `pkg-config --cflags sdl` -MMD `pkg-config --libs sdl` -lSDL_image"
    )
