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
#include "Camera.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

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

	if (SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL))
		cerr << "couldn't enable key repeat!" << endl;

	// Initialises SDL_image, an image reading wrapper on many popular formats.
	// SDL_image supports more than just these two formats. Have a look at the
	// documentation page linked in the #include comments.
	int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;

	if ((IMG_Init(flags)&img_flags) != img_flags) {
		cerr << "Failed to support JPEG and PNG image reading." << endl;
		cerr << IMG_GetError() << endl;
		SDL_Quit();
	}

	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_WarpMouse(width / 2, height / 2);
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

	terrainHeightMap = IMG_Load("Assets/heightMap_sand.png");

	// Height map

	// Define your terrain detail levels (divisions)
	float xDiv = 3;
	float zDiv = xDiv;

	cout << terrainHeightMap->w << endl;

	cout << xDiv << endl;

	// And create your height map array from the image
	// Make your divisions the same resolution as your image; it'll make your life easier.
	fillTerrainHeights(xDiv, zDiv);

	// Load objects here
	camera.setSpeed(5);
	camera.setPosition(Vec4(212, 194, 900, 1));
	camera.setTarget(Vec4(384, 55, 384, 1));

	// Create any VBO here
	createTerrain(xDiv, zDiv, &terrain, terrainHeights);

	// Add all renderable mesh to the list of objects (see header file)
	// Optional but helpful //

	// Define your projection matrix here, and your initial camera view.
	// Feel free to either use the built-in OpenGL transforms or your own ;]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ((float) width / height), 1, 2000);
	glMatrixMode(GL_MODELVIEW);

	// Enable any OpenGL feature you like, such as backface culling and depth testing, here.
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT, GL_LINE);

	glClearColor(1,1,1,0);
}

void Core::render()
{
	// Clear the frame buffer each time we draw.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColorPointer(4, GL_FLOAT, sizeof(Vec4), &colors[0](0));

	// Enable vertex array and such
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Push a new matrix to the GL_MODELVIEW stack.
	glPushMatrix();

	// Multiply the matrix by the object's transformation matrix.

	camera.load();
	// Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Pass vertex information
	glVertexPointer(4, GL_FLOAT, sizeof(Vec4), 0);

	// Pass colour information
	//glColorPointer(4, GL_FLOAT, sizeof(Vec4), 0);

	//glColor3f(1,1,1);
	// Draw object
	glDrawArrays(GL_TRIANGLES, 0, colors.size());

	// Pop the matrix from the stack.
	glPopMatrix();

	// Diable vertex array and such
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// Flip the buffer for double buffering
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	SDL_GL_SwapBuffers();
}

void Core::cleanup()
{

}

float Core::getHeight(vector<float> * heights, int x, int y, int size)
{
	return (*heights)[x + (y * size)];
}

Vec4 Core::getColor(float value, float w)
{
	Vec4 result;

	if (value < 100)
	{
		result(0) = 0;
		result(1) = 0;
		result(2) = -0.15f + value / w;
		result(3) = 1;
	}
	else if (value < 160)
	{
		result(0) = 0;
		result(1) = -0.15f + value / w;
		result(2) = 0;
		result(3) = 1;
	}
	else
	{
		for (int i=0; i < 4; i++)
		result(i) = -0.125f + value / w;
	}

	return result;
}

void Core::createTerrain(int xDiv, int zDiv, Object* _terrain, vector<float> heights)
{
	// Compute the polygon coordinates

	vector<Vec4> polygon;

	// easy way to get our image size, assumes square image
	int w = sqrt(heights.size());
	int depth = w;

	// -1 because going to the absolute last means you're going into unchartered memory.
	for (int z=0; z  < depth-1; z++)
	{
		for (int x=0; x < w; x++)
		{
			int curX = x * xDiv;
			int curZ = z * zDiv;

			polygon.push_back(Vec4(curX, getHeight(&heights, x, z, w), curZ, 1));
			polygon.push_back(Vec4(curX+xDiv, getHeight(&heights, x+1, z+1, w), curZ+zDiv, 1));
			polygon.push_back(Vec4(curX, getHeight(&heights, x, z+1, w), curZ+zDiv, 1));

			polygon.push_back(Vec4(curX, getHeight(&heights, x, z, w), curZ, 1));
			polygon.push_back(Vec4(curX+xDiv, getHeight(&heights, x+1, z, w), curZ, 1));
			polygon.push_back(Vec4(curX+xDiv, getHeight(&heights, x+1, z+1, w), curZ+zDiv, 1));

			colors.push_back(getColor(getHeight(&heights, x, z, w), w));
			colors.push_back(getColor(getHeight(&heights, x+1, z+1, w), w));
			colors.push_back(getColor(getHeight(&heights, x, z+1, w), w));

			colors.push_back(getColor(getHeight(&heights, x, z, w), w));
			colors.push_back(getColor(getHeight(&heights, x+1, z, w), w));
			colors.push_back(getColor(getHeight(&heights, x+1, z+1, w), w));
		}

	}

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, polygon.size() * sizeof(Vec4), &polygon[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	for (int i=0; i < terrainHeightMap->w; i++)
		for (int j=0; j < terrainHeightMap->h; j++)
			terrainHeights.push_back(getpixel24(terrainHeightMap, i, j)(0));

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
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
					case SDLK_w:
						camera.move(FORWARD);
						break;
					case SDLK_s:
						camera.move(BACKWARD);
						break;
					case SDLK_a:
						camera.move(LEFT);
						break;
					case SDLK_d:
						camera.move(RIGHT);
						break;
					case SDLK_SPACE:
						camera.move(UP);
						break;
					case SDLK_LSHIFT:
						camera.move(DOWN);
						break;
					case SDLK_r:
						camera.look(UP);
						break;
					case SDLK_f:
						camera.look(DOWN);
						break;
					case SDLK_q:
						camera.look(LEFT);
						break;
					case SDLK_e:
						camera.look(RIGHT);
						break;
					default: break;
				}
				break;
			case SDL_MOUSEMOTION:
				camera.handleMouse(e.motion.x, e.motion.y);
//				SDL_WarpMouse(width / 2, height / 2);
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
