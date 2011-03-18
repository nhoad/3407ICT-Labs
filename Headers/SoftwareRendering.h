#pragma once
#include "SDL.h"

class Core
{
    int width, height;
    bool running;
    SDL_Surface* buffer;

public:
    Core(int width=800, int height=600);
    virtual ~Core();

    void start();

protected:
    void initialise();
    void preprocess();
    void render();
    void handleEvents();
    void putpixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
};