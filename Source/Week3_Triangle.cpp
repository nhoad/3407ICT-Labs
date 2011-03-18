//////////////////////////////////////////////////////////////
/// Author:             <Your Name>
/// Student Number:     <s0000000>
/// Operating System:   <Windows|Mac OS X|Linux>
/// Comments:           <Notes about running your program>
//////////////////////////////////////////////////////////////

#include "Week3_Triangle.h"
#include <iostream>
#include <cmath>
using namespace std;

#if defined(__MACH__) && defined(__APPLE__)
    // Allow SDL main hack, because of the OS X Cocoa binding
#else
    // SDL main hackery disabled for Windows and Linux
    #define SDL_main main           
#endif

int main(int argc, char* argv[])
{
    Core example;
    example.start();
    return 0;
}

Core::Core(int width, int height) : running(true)
{
    this->width = width;
    this->height = height;
}

Core::~Core()
{
    // Allows SDL to exit properly
    if (SDL_WasInit(SDL_INIT_VIDEO))
        SDL_Quit();
}

void Core::start()
{
    // Setup and then enter main loop
    initialise();
    while (running) {
        render();
        handleEvents();
    }
}

void Core::initialise()
{
    // Initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << SDL_GetError() << endl;
        SDL_Quit();
    }

    // Create a framebuffer
    Uint32 flags = SDL_SWSURFACE | SDL_DOUBLEBUF;

    buffer = SDL_SetVideoMode(width, height, 32, flags);
    if (!buffer) {
        cerr << SDL_GetError() << endl;
        SDL_Quit();
    }
}

void Core::render()
{
    // Clear the screen to black
    SDL_FillRect(buffer, NULL, 0);

    SDL_LockSurface(buffer);
    /////////////////////////////////////
    // Draw objects here
    float scale = 256.f;    // Size of the triangle
    float half = scale/2;

    Point a(0, (int)-scale, 255, 0, 0);
    Point b((int)(half*sqrt(3.f)), (int)half, 0, 255, 0);
    Point c((int)(-half*sqrt(3.f)), (int)half, 0, 0, 255);

    a.x += width/2;     a.y += height/2;
    b.x += width/2;     b.y += height/2;
    c.x += width/2;     c.y += height/2;

    // Draw a RGB Equilateral triangle.
    drawTriangle(a, b, c);

    /////////////////////////////////////
    SDL_UnlockSurface(buffer);
    // Flip the buffers
    SDL_Flip(buffer);
}

/////////////////////////////////////
// Lab Exercise

/////////////////////////////////////
// Renders a triangle(a, b, c) to buffer
void Core::drawTriangle(Point a, Point b, Point c)
{

}

/////////////////////////////////////
// "draws" the line(a, b) but stores the result in the vector.
// This can be used to create your edge lists.
// You can keep doing push_back() into the std::vector, but make sure that your lists are
// of equal length after interpolation!
void Core::makeLine(Point a, Point b, vector<Point> *line)
{

}

/////////////////////////////////////
// Renders the line(a, b). Assume that a and b have the same y value.
void Core::scanline(Point a, Point b)
{

}

/////////////////////////////////////
// Clips the triangle against the screen boundary. May return a quad.
std::vector<Point> Core::clip(std::vector<Point>)
{

}

/////////////////////////////////////
// Decomposes a polygon (if more than a triangle) into multiple triangles.
std::vector<Point> Core::decompose(std::vector<Point>)
{

}

/////////////////////////////////////

int Core::sign(int x)
{
    if (x < 0)
        return -1;
    else if (x > 0)
        return 1;
    return 0;
}

int Core::round(float x)
{
    return int(floor(x + 0.5f));
}

float Core::lerp(float a, float b, float ratio)
{
    if (a == b || ratio == 0.f)
        return float(a);
    if (ratio == 1.0f)
        return float(b);
    return a + ratio * (b - a);
}

void Core::handleEvents()  
{  
    SDL_Event e;  
    while (SDL_PollEvent(&e)) {  
        switch (e.type) {  
            case SDL_QUIT:  
                running = false;  
            break;  
            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                    break;
                    default: break;
                }
            break;
            case SDL_MOUSEBUTTONDOWN: // Do nothing this time
            break;
            default: break;  
        }  
    }  
}  

void Core::putpixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    Uint8 *p = (Uint8 *)buffer->pixels + y * buffer->pitch + x * 4;

#if defined(__MACH__) && defined(__APPLE__)
    *(Uint32 *)p = b << 24 | g << 16 | r << 8 | a; // Big endian
#else
    *(Uint32 *)p = b | g << 8 | r << 16 | a << 24; // Lil endian
#endif
}