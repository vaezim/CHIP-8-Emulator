#include <algorithm>

#include "display.h"


Display::~Display() {
    SDL_QuitSubSystem(USED_SDL_SUBSYSTEMS);
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
    if (SDL_InitSubSystem(USED_SDL_SUBSYSTEMS) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
                     "Failed to initialize SDL subsystems. %s", SDL_GetError());
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

void Display::DrawPixels() {
    // According to SDL wiki, SDL_UpdateTexture() is slower than
    // SDL_LockTexture()/SDL_UnlockTexture() which are recommended for
    // higher resolutions. However as this emulator uses 2048 pixels (8Kb),
    // copying it to GPU VRAM at 60FPS is not too slow.
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
}

void Display::ClearScreen() {
    std::fill(m_pixels, m_pixels + NUM_PIXELS, BLACK);
    DrawPixels();
}
