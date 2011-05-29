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

#include "Core.h"
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
	elapsedTime(0), running(true), camera(FPS)
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
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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

	preprocess();

	double avg = 0;
	int count = 0;

	while (running) {
		render();
		handleEvents();

		elapsedTime = t.getSeconds();
		t.start();

		if (avg < 1)
		{
			avg += elapsedTime;
			count++;
		}
		else
		{
			SDL_WM_SetCaption(string("Pacman, by Nathan Hoad 2011 - FPS: " + typeToString<int>(count)).c_str(), "");
			avg = 0;
			count = 0;
		}

	}
	cleanup();
}

void Core::preprocess()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ((float) width / height), 1, 2000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera.setPosition(Vec3(1250, 1000, -300));
	camera.setViewAngle(45, 225, 0);

	game = new PacmanGame();
	game->initialise();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glDepthMask(GL_TRUE);
}

void Core::render()
{
	// Clear the frame buffer each time we draw.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable vertex array and such
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Push a new matrix to the GL_MODELVIEW stack.
	glPushMatrix();
	camera.load();

	game->draw();

	glPopMatrix();

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	SDL_GL_SwapBuffers();
}

void Core::cleanup()
{
	delete game;
}

void Core::handleEvents()
{
	game->update(elapsedTime);

	if (game->getGameState() == GAME_QUIT)
		running = false;
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
