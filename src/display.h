#ifndef DISPLAY_H
#define DISPLAY_Y

#include <SDL2/SDL.h>

#include "defs.h"


class Display {
public:
    Display() = default;
    ~Display();

    Error_t Initialize();
    bool ShouldRun();

    void DrawPixels();
    void ClearScreen();

    inline void FlipPixel(int X, int Y) {
        m_pixels[Y * PIXELS_PER_WIDTH + X] = ~m_pixels[Y * PIXELS_PER_WIDTH + X];
    }
    inline uint32_t GetPixel(int X, int Y) const {
        return m_pixels[Y * PIXELS_PER_WIDTH + X];
    }

    uint8_t GetPressedKey();
    bool IsKeyPressed(uint8_t key);

private:
    SDL_Window *m_window = NULL;
    SDL_Texture *m_texture = NULL;
    SDL_Renderer *m_renderer = NULL;

    uint32_t m_pixels[NUM_PIXELS] = {};
};

#endif // DISPLAY_H
