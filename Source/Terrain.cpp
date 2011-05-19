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

	SDL_WM_GrabInput(SDL_GRAB_ON);
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
	while (running) {
		render();
		handleEvents();
		SDL_WM_SetCaption(string("Pacman, by Nathan Hoad 2011 - FPS: " + typeToString<int>(t.getFPS(1))).c_str(), "");
		elapsedTime = t.getSeconds();
		t.start();
	}
	cleanup();
}

void Core::preprocess()
{
	for (int i=0; i < 256; i++)
		keys[i] = false;

	terrain = Loader::loadTerrain("Assets/pacmanHeightmap.png", 3);

	// Load objects here
	camera.setSpeed(1);
	camera.setPosition(Vec3(1600, 1300, 720));
	camera.setViewAngle(60, 270, 0);

	// Define your projection matrix here, and your initial camera view.
	// Feel free to either use the built-in OpenGL transforms or your own ;]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ((float) width / height), 1, 2000000);
	glMatrixMode(GL_MODELVIEW);

	// Enable any OpenGL feature you like, such as backface culling and depth testing, here.
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT, GL_LINE);

	objects.push_back(new Object("Assets/Cube.obj"));
	objects[0]->buffer();
	objects[0]->matrix = Mat4::scale(5,5,5);
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

	// Multiply the matrix by the object's transformation matrix.

	camera.load();
	// Bind the VBO
	// Pass vertex information

	terrain.draw();

	glColor3f(1,1,1);
	for (int i=0; i < objects.size(); i++)
		objects[i]->draw();

	glPopMatrix();

	// Pop the matrix from the stack.

	// Diable vertex array and such
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// Flip the buffer for double buffering
	SDL_GL_SwapBuffers();

}

void Core::cleanup()
{

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
/*				switch (e.key.keysym.sym) {
					keys[
					case SDLK_w:
						keys[SDLK_w] = true;
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
				break;*/
			case SDL_MOUSEMOTION:
				camera.handleMouse(e.motion.xrel, e.motion.yrel);
//				SDL_WarpMouse(width / 2, height / 2);
			default: break;
		}
	}

	if (keys[SDLK_w])
		camera.move(FORWARD);

	if (keys[SDLK_s])
		camera.move(BACKWARD);

	if (keys[SDLK_a])
		camera.move(LEFT);

	if (keys[SDLK_d])
		camera.move(RIGHT);

	if (keys[SDLK_s])
		camera.move(BACKWARD);
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
