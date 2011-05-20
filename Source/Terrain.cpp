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
#include "Mat4.h"
#include "Camera.h"
#include "Loader.h"
#include "Pacman.h"
#include "StringFunctions.h"

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
	elapsedTime(0), running(true), terrain(), camera(FPS)//, objects()
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

	/*if (SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL))
		cerr << "couldn't enable key repeat!" << endl;
*/
	// Initialises SDL_image, an image reading wrapper on many popular formats.
	// SDL_image supports more than just these two formats. Have a look at the
	// documentation page linked in the #include comments.
	int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;

	if ((IMG_Init(flags)&img_flags) != img_flags) {
		cerr << "Failed to support JPEG and PNG image reading." << endl;
		cerr << IMG_GetError() << endl;
		SDL_Quit();
	}

//	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(SDL_DISABLE);
}

///
/// The Main Loop
///
void Core::start()
{
	initialise();
	Time t;
	t.start();
	int frameCount = 0;
	float oldTime = 0;

	preprocess();

	while (running) {
		render();
		handleEvents();

		frameCount++;

		oldTime = elapsedTime;
		t.start();

		timeframe = 0.001;

		if (frameCount == 30)
		{
			SDL_WM_SetCaption(string("Pacman, by Nathan Hoad 2011 - FPS: " + typeToString<int>(t.getFPS(1))).c_str(), "");
			frameCount = 0;
		}

	}
	cleanup();
}

void Core::preprocess()
{
	for (int i=0; i < 256; i++)
		keys[i] = false;

	terrain = Loader::loadTerrain("Assets/pacman.png", 4);

	camera.setSpeed(0.1);
	camera.setPosition(Vec3(80, 300, 70));
	camera.setViewAngle(90, 270, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ((float) width / height), 1, 2000000);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);

	objects.push_back(new GameEntity("Assets/Cube.obj"));

	player = new Pacman();
	player->obj->buffer();

	//food = Pacman::loadFood("Assets/Cube.obj", 50);
}

void Core::render()
{
	// Clear the frame buffer each time we draw.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable vertex array and such
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	// Push a new matrix to the GL_MODELVIEW stack.
	glPushMatrix();

	// load the camera's matrix to set our position
	camera.load();

	// draw our terrain
	terrain.draw();

	glDisableClientState(GL_COLOR_ARRAY);

	for (int i=0; i < objects.size(); i++)
		objects[i]->draw();

	player->draw(timeframe);

	/*for (int i=0; i < food.size(); i++)
		food[i]->draw();
*/
	glPopMatrix();

	// Pop the matrix from the stack.

	// Diable vertex array and such
	glDisableClientState(GL_VERTEX_ARRAY);

	// Flip the buffer for double buffering
	SDL_GL_SwapBuffers();
}

void Core::cleanup()
{
	for (int i=0; i < objects.size(); i++)
		delete objects[i];
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
				keys[e.key.keysym.sym] = false;
				if (SDLK_ESCAPE == e.key.keysym.sym)
					running = false;
				break;
			case SDL_KEYDOWN:
				keys[e.key.keysym.sym] = true;
				break;
			case SDL_MOUSEMOTION:
				camera.handleMouse(e.motion.xrel, e.motion.yrel);
			default: break;
		}
	}

	if (keys[SDLK_UP])
		camera.move(FORWARD);

	if (keys[SDLK_DOWN])
		camera.move(BACKWARD);

	if (keys[SDLK_LEFT])
		camera.move(LEFT);

	if (keys[SDLK_RIGHT])
		camera.move(RIGHT);

	if (keys[SDLK_w])
		player->move(UP);

	if (keys[SDLK_s])
		player->move(DOWN);

	if (keys[SDLK_a])
		player->move(LEFT);

	if (keys[SDLK_d])
		player->move(RIGHT);

}

Vec3 Core::getpixel24(SDL_Surface *surface, int x, int y)
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
	float result[] = {r, g, b};
	return Vec3(result); // You may have to change this, depending on your data structure
}
