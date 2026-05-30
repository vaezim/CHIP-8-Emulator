
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

set(SRC_FILES
    "main.cpp"
    "defs.h"
    "cpu.h"
    "cpu.cpp"
    "display.h"
    "display.cpp"
    "motherboard.h"
    "motherboard.cpp"
)

add_executable(${PROJECT_BINARY_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_BINARY_NAME} SDL2 SDL2_mixer)
