#include <algorithm>

#include "display.h"


Display::~Display() {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
    SDL_Quit();
}

Error_t Display::Initialize() {

    // Initialize SDL subsystems
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
                     "Failed to initialize SDL video subsystem. %s", SDL_GetError());
        return ERROR_CODE;
    }

    // Create window
    m_window = SDL_CreateWindow(WINDOW_TITLE,
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_HIDDEN);
    if (!m_window) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
                     "Failed to create window. %s", SDL_GetError());
        return ERROR_CODE;
    }

    // Create texture renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER,
                     "Failed to create renderer. %s", SDL_GetError());
        return ERROR_CODE;
    }

    // Create the pixelated monochrome texture
    m_texture = SDL_CreateTexture(m_renderer,
                                  SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                  PIXELS_PER_WIDTH, PIXELS_PER_HEIGHT);
    if (!m_texture) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER,
                     "Failed to create texture. %s", SDL_GetError());
        return ERROR_CODE;
    }

    // Pixels per width/height are an integer factor (20) of window width/height
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest")) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER,
                     "Failed to set render hint. %s", SDL_GetError());
        return ERROR_CODE;
    }
    if (SDL_RenderSetIntegerScale(m_renderer, SDL_TRUE) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER,
                     "Failed to set render integer scale. %s", SDL_GetError());
        return ERROR_CODE;
    }

    SDL_ShowWindow(m_window);
    return SUCCESS;
}

bool Display::ShouldRun() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                return false;
            }
            case SDL_KEYDOWN: {
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE: {
                        return false;
                    }
                    default: {
                        break;
                    }
                }
            }
            default: {
                break;
            }
        }
    }
    return true;
}

/**
 * According to SDL docs, SDL_UpdateTexture() is slower than
 * SDL_LockTexture()/SDL_UnlockTexture() which are recommended for
 * higher resolutions. However as this emulator uses 2048 pixels (8Kb),
 * copying it to GPU VRAM at 60FPS is not too slow.
 */
void Display::RenderPixels() {
    if (SDL_UpdateTexture(m_texture, NULL, m_pixels, PIXELS_PER_WIDTH * sizeof(uint32_t)) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER,
                     "Failed to update texture. %s", SDL_GetError());
        return;
    }
    if (SDL_RenderCopy(m_renderer, m_texture, NULL, NULL) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER,
                     "Failed to copy texture to renderer. %s", SDL_GetError());
        return;
    }
    SDL_RenderPresent(m_renderer);
    m_screenUpdated = false;
}

void Display::ClearPixels() {
    std::fill(m_pixels, m_pixels + NUM_DISPLAY_PIXELS, BLACK);
}

/**
 * [1] => (0x1) | [2] => (0x2) | [3] => (0x3) | [4] => (0xC)
 * [Q] => (0x4) | [W] => (0x5) | [E] => (0x6) | [R] => (0xD)
 * [A] => (0x7) | [S] => (0X8) | [D] => (0x9) | [F] => (0xE)
 * [Z] => (0xA) | [X] => (0x0) | [C] => (0xB) | [V] => (0xF)
 */
uint8_t Display::GetPressedKey() {
    SDL_PumpEvents();
    auto state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_1]) return 0x1;
    if (state[SDL_SCANCODE_2]) return 0x2;
    if (state[SDL_SCANCODE_3]) return 0x3;
    if (state[SDL_SCANCODE_4]) return 0xC;
    if (state[SDL_SCANCODE_Q]) return 0x4;
    if (state[SDL_SCANCODE_W]) return 0x5;
    if (state[SDL_SCANCODE_E]) return 0x6;
    if (state[SDL_SCANCODE_R]) return 0xD;
    if (state[SDL_SCANCODE_A]) return 0x7;
    if (state[SDL_SCANCODE_S]) return 0x8;
    if (state[SDL_SCANCODE_D]) return 0x9;
    if (state[SDL_SCANCODE_F]) return 0xE;
    if (state[SDL_SCANCODE_Z]) return 0xA;
    if (state[SDL_SCANCODE_X]) return 0x0;
    if (state[SDL_SCANCODE_C]) return 0xB;
    if (state[SDL_SCANCODE_V]) return 0xF;
    return UINT8_MAX;
}

/**
 * (0x1) => [1] | (0x2) => [2] | (0x3) => [3] | (0xC) => [4]
 * (0x4) => [Q] | (0x5) => [W] | (0x6) => [E] | (0xD) => [R]
 * (0x7) => [A] | (0X8) => [S] | (0x9) => [D] | (0xE) => [F]
 * (0xA) => [Z] | (0x0) => [X] | (0xB) => [C] | (0xF) => [V]
 */
bool Display::IsKeyPressed(uint8_t key) {
    static constexpr SDL_Scancode keyMap[] = {
        SDL_SCANCODE_X, // 0x0
        SDL_SCANCODE_1, // 0x1
        SDL_SCANCODE_2, // 0x2
        SDL_SCANCODE_3, // 0x3
        SDL_SCANCODE_Q, // 0x4
        SDL_SCANCODE_W, // 0x5
        SDL_SCANCODE_E, // 0x6
        SDL_SCANCODE_A, // 0x7
        SDL_SCANCODE_S, // 0x8
        SDL_SCANCODE_D, // 0x9
        SDL_SCANCODE_Z, // 0xA
        SDL_SCANCODE_C, // 0xB
        SDL_SCANCODE_4, // 0xC
        SDL_SCANCODE_R, // 0xD
        SDL_SCANCODE_F, // 0xE
        SDL_SCANCODE_V  // 0xF
    };
    if (key > 0xF) {
        SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Key code %d is larger than 0xF.", key);
        return false;
    }
    SDL_PumpEvents();
    auto state = SDL_GetKeyboardState(NULL);
    return static_cast<bool>(state[keyMap[key]]);
}
