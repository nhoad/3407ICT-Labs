#pragma once
#include "SDL.h"
#include <vector>

class Point
{
public:
    int x;
    int y;
	Uint8 r;
	Uint8 g;
	Uint8 b;
    Point(int x, int y, Uint8 r, Uint8 g, Uint8 b) : x(x), y(y), r(r), g(g), b(b) {}
};

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

    void drawTriangle(Point a, Point b, Point c);
    void makeLine(Point a, Point b, std::vector<Point> *line);
    void scanline(Point a, Point b);
    std::vector<Point> clip(std::vector<Point>);
    std::vector<Point> decompose(std::vector<Point>);
    
    int sign(int x);
    int round(float x);
    float lerp(float a, float b, float ratio);
};