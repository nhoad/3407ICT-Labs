//////////////////////////////////////////////////////////////
/// Author:             <Your Name>
/// Student Number:     <s0000000>
/// Operating System:   <Windows|Mac OS X|Linux>
/// Comments:           <Notes about running your program>
//////////////////////////////////////////////////////////////
#if defined(__MACH__) && defined(__APPLE__)
// Allow SDL main hack, because of the OS X Cocoa binding
#else
// SDL main hackery disabled for Windows and Linux
#define SDL_main main
#endif

// SDL's glext.h version disabled
#define NO_SDL_GLEXT

#include "Terrain.h"
#include "HPTime.h"
#include "Transformation.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

///
/// Program Entry
///
int main(int argc, char* argv[])
{
	Core example;
	example.start();
	return 0;
}

///
/// Constructors and Destructors
///
	Core::Core(int width, int height, bool fullscreen)
: width(width), height(height), fullscreen(fullscreen),
	elapsedTime(0), running(true), terrain()//, objects()
{
}

Core::~Core()
{
	if (TTF_WasInit())
		TTF_Quit();
	if (SDL_WasInit(SDL_INIT_VIDEO))
		SDL_Quit();
	IMG_Quit();
}

///
/// Individual Functions
///
void Core::initialise()
{
	// Initialise SDL with C-style error checking
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cerr << SDL_GetError() << endl;
		SDL_Quit();
	}

	// Create a render buffer through SDL
	Uint32 flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL;
	if (fullscreen)
		flags |= SDL_FULLSCREEN;
	SDL_Surface* buffer = SDL_SetVideoMode(width, height, 32, flags);
	if (!buffer) {
		cerr << SDL_GetError() << endl;
		SDL_Quit();
	}

	// Initialise GLEW for OGL extension bindings
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		cerr << glewGetErrorString(err) << endl;
		SDL_Quit();
	}

	// Initialise SDL_ttf, a truetype wrapper for SDL
	if (TTF_Init() < 0) {
		cerr << TTF_GetError() << endl;
		TTF_Quit();
		SDL_Quit();
	}

	// Initialises SDL_image, an image reading wrapper on many popular formats.
	// SDL_image supports more than just these two formats. Have a look at the
	// documentation page linked in the #include comments.
	int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;

	if ((IMG_Init(flags)&img_flags) != img_flags) {
		cerr << "Failed to support JPEG and PNG image reading." << endl;
		cerr << IMG_GetError() << endl;
		SDL_Quit();
	}
}

///
/// The Main Loop
///
void Core::start()
{
	initialise();
	Time t;
	t.start();

	preprocess();
	while (running) {
		render();
		handleEvents();
		elapsedTime = t.getSeconds();
		t.start();
	}
	cleanup();
}

void Core::preprocess()
{
	// Load any images here

	cout << "asdf" << endl;
	terrainHeightMap = IMG_Load("Assets/heightMap_sand.png");

	// Height map
	for (int i=0; i < terrainHeightMap->w; i++)
		for (int j=0; j < terrainHeightMap->h; j++)
			terrainHeights.push_back(getpixel24(terrainHeightMap, i, j)(0));

	for (int i=0; i < terrainHeights.size(); i++)
		cout << "height of " << i << " " << terrainHeights[i] << endl;

	// Define your terrain detail levels (divisions)
	// ..

	// And create your height map array from the image
	// Make your divisions the same resolution as your image; it'll make your life easier.
//	fillTerrainHeights(/**/);

	// Load objects here

	// Create any VBO here
	//createTerrain(/**/);

	// Add all renderable mesh to the list of objects (see header file)
	// Optional but helpful //

	// Define your projection matrix here, and your initial camera view.
	// Feel free to either use the built-in OpenGL transforms or your own ;]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ((float) width / height), 1, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1, 1, 1, 0, 0, 0, 0, 1, 0);

	// Enable any OpenGL feature you like, such as backface culling and depth testing, here.
	// ..
	//glPolygonMode(GL_FRONT, GL_LINE);
}

void Core::render()
{
	// Clear the frame buffer each time we draw.
	// ..

	// Enable vertex array and such
	// ..

	// Push a new matrix to the GL_MODELVIEW stack.
	glPushMatrix();

	// Multiply the matrix by the object's transformation matrix.
	// ..

	// Bind the VBO
	// ..

	// Pass vertex information

	// Pass colour information
	// ..

	// Draw object
	// ..

	// Pop the matrix from the stack.
	glPopMatrix();

	// Diable vertex array and such
	// ..

	// Flip the buffer for double buffering
	// ..
}

void Core::cleanup()
{
}

void Core::createTerrain(int xDiv, int zDiv, Object* _terrain, float* heights)
{
	// Compute the polygon coordinates
	// ..

	/*
		.
		.
		| / | /
		X---X---
		| / | /
		O---X---..
		Adds 6 vertices (2 triangles) for this section of quad.
		*/
	// ..

	// Send the generated terrain data to the terrain VBO.
	// ..
}

void Core::fillTerrainHeights(int xDiv, int zDiv)
{
	// To access the pixel in SDL_Surface, you need to lock it; see Week 3 lab.
	SDL_LockSurface(terrainHeightMap);

	// Copy the pixel value (just use one of either R, G, or B channel) into the height map array
	// .. (use getpixel here)

	// Remember to unlock it.
	SDL_UnlockSurface(terrainHeightMap);
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

Vec4 Core::getpixel24(SDL_Surface *surface, int x, int y)
{
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
#if defined(__MACH__) && defined(__APPLE__)
	float b = p[0];
	float g = p[1];
	float r = p[2]; // Big endian
#else
	float b = p[2];
	float g = p[1];
	float r = p[0]; // little endian
#endif
	float result[] = {r, g, b, 1};
	return Vec4(result); // You may have to change this, depending on your data structure
}
