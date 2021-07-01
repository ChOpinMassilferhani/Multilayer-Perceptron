add_executable(
    Executable
    main.c
)

find_package(SDL2 REQUIRED)
find_package(SDL REQUIRED)






set_target_properties(
    Executable
    PROPERTIES
    C_STANDARD 99
    C_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)


include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})


target_link_libraries(
    Executable
    PUBLIC
    ImageProcessing
    Segmentation
    Network
    OCR
    Tools
    m
    ${SDL2_LIBRARY}
    ${SDL_LIBRARY}
    ${SDL_IMAGE_LIBRARY}
)
