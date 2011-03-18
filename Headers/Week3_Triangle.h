#pragma once
#include "SDL.h"
#include <vector>
using std::vector;

class Point
{
	public:
		int x;
		int y;

		Uint8 r;
		Uint8 g;
		Uint8 b;

		Point(int x, int y, Uint8 r=0, Uint8 g=0, Uint8 b=0) : x(x), y(y), r(r), g(g), b(b) {}
};

int compareOnX(Point a, Point b);
int compareOnY(Point a, Point b);

class Core
{
	int width, height;
	bool running;
	SDL_Surface* buffer;
	Point point;

	public:
	Core(int width=800, int height=600);
	virtual ~Core();

	void start();

	protected:
	void initialise();
	void preprocess();
	void render();
	void handleEvents();

	vector<Point> makeLine(Point a, Point b);
	void triangle(Point a, Point b, Point c);
	void scanLine(Point a, Point b);
	void putpixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
	void draw(Point a, Point b);
	void line(Point a, Point b);
};
