//////////////////////////////////////////////////////////////
/// Author:             <Your Name>
/// Student Number:     <s0000000>
/// Operating System:   <Windows|Mac OS X|Linux>
/// Comments:           <Notes about running your program>
//////////////////////////////////////////////////////////////

#include "Week2_LineDrawing.h"
#include <iostream>
#include <algorithm>
#define round(x) (int) x + 0.5
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

Core::Core(int width, int height) : running(true), point(0, 0, 255)
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

	// Clear the screen to black
	SDL_FillRect(buffer, NULL, 0);
	SDL_Flip(buffer);
}

void Core::draw(Point a, Point b)
{
	SDL_LockSurface(buffer);
	/////////////////////////////////////
	// Draw objects here
	line(a, b);

	/////////////////////////////////////
	SDL_UnlockSurface(buffer);
	// Flip the buffers
	SDL_Flip(buffer);
}

void Core::line(Point a, Point b)
{
	////////////////////////////////////////////
	// Lab Exercise
	//
	//

	cout << "line!" << endl;

	if (a.x > b.x)
		swap(a, b);

	// we want to store x and y as floats or it won't work!
	double y = a.y;
	double x = a.x;

	// store colours for same reason
	double red = a.r;
	double green = a.g;
	double blue = a.b;

	double slope = ((double)(b.y - a.y)) / (double)(b.x - a.x);

	if (slope <= 1.0 && slope > -1.0)
	{
		double r_inc = (double) (b.r - a.r) / (b.x - a.x);
		double g_inc = (double) (b.g - a.g) / (b.x - a.x);
		double b_inc = (double) (b.b - a.b) / (b.x - a.x);

		for (; a.x < b.x; a.x++)
		{
			putpixel(a.x, (int) round(y), (int) round(red), (int) round(green), (int) round(blue));

			red += r_inc;
			green += g_inc;
			blue += b_inc;
			y += slope;
		}
	}
	else if (slope <= -1.0)
	{
		// because we're drawing a steep negative slope, swap a and b back to their original positions.
		swap(a.x, b.x);
		swap(a.y, b.y);

		y = a.y;
		x = a.x;

		double r_inc = (double) (b.r - a.r) / (b.y - a.y);
		double g_inc = (double) (b.g - a.g) / (b.y - a.y);
		double b_inc = (double) (b.b - a.b) / (b.y - a.y);

		for (; a.y < b.y; a.y++)
		{
			putpixel((int) round(x), a.y, (int) round(red), (int) round(green), (int) round(blue));

			red += r_inc;
			green += g_inc;
			blue += b_inc;
			x += 1 / slope;
		}
	}
	else
	{
		double r_inc = (double) (b.r - a.r) / (b.y - a.y);
		double g_inc = (double) (b.g - a.g) / (b.y - a.y);
		double b_inc = (double) (b.b - a.b) / (b.y - a.y);

		for (; a.y < b.y; a.y++)
		{
			putpixel((int) round(x), a.y, (int) round(red), (int) round(green), (int) round(blue));

			red += r_inc;
			green += g_inc;
			blue += b_inc;
			x += 1 / slope;
		}


		////////////////////////////////////////////
	}
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
			case SDL_MOUSEBUTTONDOWN:
				draw(point, Point(e.button.x, e.button.y, 0,0, 255));
				point.x = e.button.x;
				point.y = e.button.y;
				point.b = point.r;
				point.r = point.g;
				point.g = point.b;
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
