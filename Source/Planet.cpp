/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#include "Planet.h"
#include <cmath>
#define PI 3.1415

#include "Transformation.h"

#if defined(__MACH__) && defined(__APPLE__)
// Allow SDL main hack, because of the OS X Cocoa binding
#else
// SDL main hackery disabled for Windows and Linux
#define SDL_main main
#endif

// SDL's glext.h version disabled
#define NO_SDL_GLEXT

// http://www.libsdl.org/
#include "SDL.h"
// http://www.libsdl.org/projects/SDL_ttf/
#include "SDL_ttf.h"
// http://glew.sourceforge.net/
#include "glew.h"

#include "HPTime.h"

#include <iostream>

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
Core::Core(int width, int height, bool fullscreen) : elapsedTime(0), running(true)
{
	this->width = width;
	this->height = height;
	this->fullscreen = fullscreen;
}

Core::~Core()
{
	if (TTF_WasInit())
		TTF_Quit();
	if (SDL_WasInit(SDL_INIT_VIDEO))
		SDL_Quit();
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
}

void Core::preprocess()
{
	// Define or load objects here
	angle = 0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-0.5, -0.5, -0.5);

	GLfloat values[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, values);
	cout << Mat4(values) << endl;

	exit(0);
}

void Core::planet(double r)
{
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
		for (float i=0; i < 2 * PI; i += 0.15)
			glVertex3f(r * cos(i), r * sin(i), 0.0f);
	glEnd();
	glPopMatrix();
}

void Core::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//translate along the x-axis
	glColor3f(0.0f,0.0f,1.0f); //blue color

	angle += 0.01;

	if (angle >= 360)
		angle = 0;

	glMatrixMode(GL_MODELVIEW);
	glRotatef(angle, 0.0f, 0.0f, 0.1f);

	planet(0.2); // earth

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.4,-0.4f,0.0f);
	glRotatef(angle, 0.0f, 0.0f, 0.1f);
	glColor3f(0.737f,0.56f,0.56f); //blue color

	planet(0.1); // moon

	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.15f,-0.15f,0.0f);
	glRotatef(angle, 0.0f, 0.0f, 0.1f);
	glColor3f(0.0,1.0f,0.0f); //blue color

	planet(0.05); // tiny moon!

	SDL_GL_SwapBuffers();
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
