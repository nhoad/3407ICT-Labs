#pragma once
#include "SDL.h"

class Core
{
public:
    Core(int width=800, int height=600);
    virtual ~Core();

    void start();

protected:
    int width, height;
	 double elapsedTime;
    bool running;
    SDL_Surface* buffer;

    void initialise();
    virtual void preprocess();
    virtual void render();
    void handleEvents();
    void putpixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
};
