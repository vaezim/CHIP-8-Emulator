#ifndef DEFS_H
#define DEFS_H

#include <cstdint>


#define WINDOW_TITLE    "CHIP-8 Emulator"

// Memory addresses
#define MEMORY_SIZE_BYTES   4096
#define ROM_START_ADDRESS   0x200
#define FONTS_START_ADDRESS 0x050

// FPS and CPU clocks/s
constexpr int TARGET_FPS = 60;
constexpr int TARGET_MLLISECONDS_PER_FRAME = 1000 / TARGET_FPS;
constexpr int TARGET_INSTRUCTIONS_PER_SECOND = 700;
constexpr int TARGET_INSTRUCTIONS_PER_FRAME = TARGET_INSTRUCTIONS_PER_SECOND / TARGET_FPS;

// Error type
typedef int Error_t;
constexpr Error_t SUCCESS = 0;
constexpr Error_t ERROR_CODE = 1;

// Pixels
constexpr int PIXELS_PER_WIDTH = 64;
constexpr int PIXELS_PER_HEIGHT = 32;
constexpr int NUM_DISPLAY_PIXELS = PIXELS_PER_WIDTH * PIXELS_PER_HEIGHT;

// Window size
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = WINDOW_WIDTH / (PIXELS_PER_WIDTH / PIXELS_PER_HEIGHT);

// Colors
constexpr uint32_t WHITE = UINT32_MAX; // RGBA
constexpr uint32_t BLACK = 0;

// Fonts
constexpr int BYTES_PER_FONT = 5;
constexpr int FONTS_SIZE = 16 * BYTES_PER_FONT;
constexpr uint8_t FONTS[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

#endif // DEFS_H
