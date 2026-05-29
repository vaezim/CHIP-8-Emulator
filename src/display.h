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

    void ClearScreen();

private:
    SDL_Window *m_window = NULL;
    SDL_Texture *m_texture = NULL;
    SDL_Renderer *m_renderer = NULL;

    uint32_t m_pixels[NUM_PIXELS];
};

#endif // DISPLAY_H
