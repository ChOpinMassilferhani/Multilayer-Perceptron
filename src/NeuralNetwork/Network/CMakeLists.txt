add_library(
    Network
    STATIC
    save.c
    network.c
)

target_include_directories(
    Network
    PUBLIC
    .
    ../Tools
    )

target_link_libraries(
    Network
    PUBLIC
    Tools
    )

set_target_properties(
    Network
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    )

target_compile_definitions(
    Network
    PUBLIC
    "_GNU_SOURCE"
    )
