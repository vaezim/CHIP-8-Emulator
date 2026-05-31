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

    void ClearPixels();
    void RenderPixels();

    // Used by CPU to signal Display to re-render pixels
    inline void ScreenIsUpdated() { m_screenUpdated = true; }
    inline bool ShouldUpdateScreen() { return m_screenUpdated; }

    inline void FlipPixel(int X, int Y) {
        m_pixels[Y * PIXELS_PER_WIDTH + X] = ~m_pixels[Y * PIXELS_PER_WIDTH + X];
    }
    inline uint32_t GetPixel(int X, int Y) const {
        return m_pixels[Y * PIXELS_PER_WIDTH + X];
    }

    uint8_t GetPressedKey();
    bool IsKeyPressed(uint8_t key);

private:
    bool m_screenUpdated{ false };

    SDL_Window *m_window = nullptr;
    SDL_Texture *m_texture = nullptr;
    SDL_Renderer *m_renderer = nullptr;

    uint32_t m_pixels[NUM_DISPLAY_PIXELS] = {};
};

#endif // DISPLAY_H
