cmake_minimum_required(VERSION 3.5)
project(chip8-emulator)

set(PROJECT_BINARY_NAME "chip8-emulator")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_CXX_FLAGS 
    "-Wall -Werror -Wextra -Wshadow -Wnon-virtual-dtor -Wunused -pedantic"
)

add_subdirectory(ROMs)
add_subdirectory(audio)
add_subdirectory(src)
