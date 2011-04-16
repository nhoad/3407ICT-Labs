/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 */
#include "SoftwareRendering.h"
#include <iostream>
using namespace std;

#include "HPTime.h"

#if defined(__MACH__) && defined(__APPLE__)
// Allow SDL main hack, because of the OS X Cocoa binding
#else
// SDL main hackery disabled for Windows and Linux
#define SDL_main main
#endif

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
	preprocess();

	Time t;
	t.start();

	while (running) {
		render();
		handleEvents();
		elapsedTime = t.getSeconds();
		t.start();
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

void Core::preprocess()
{

}

void Core::render()
{
	// Clear the screen to black
	SDL_FillRect(buffer, NULL, 0);

	SDL_LockSurface(buffer);
	/////////////////////////////////////
	// Draw objects here

	cout << "I do nothing!" << endl;

	/////////////////////////////////////
	SDL_UnlockSurface(buffer);
	// Flip the buffers
	SDL_Flip(buffer);
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
