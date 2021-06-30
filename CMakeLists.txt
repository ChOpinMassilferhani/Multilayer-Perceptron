cmake_minimum_required(VERSION 3.0)
project(42sh VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_FLAGS_ASAN
    "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno- +++omit-frame-pointer -g "
    CACHE STRING "Flags used by the C compiler during AddressSanitizer builds."  
    FORCE)
set(CMAKE_CXX_FLAGS_ASAN
    "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno- +++omit-frame-pointer -g "
    CACHE STRING "Flags used by the C++ compiler during AddressSanitizer builds."
    FORCE)


if(NEED_COVERAGE)
    add_compile_options(-Wall -Wextra -pedantic -Werror -g --coverage)
    add_link_options(--coverage)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -g)
endif()

add_subdirectory(src/ImageProcessing)
