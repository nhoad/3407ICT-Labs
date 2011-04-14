/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 */
#include "RGBCube.h"
#include "ObjectLoader.h"
#include "Transformation.h"
#define round(x) (int) x + 0.5

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
	// build the cube
	ObjectLoader loader;

	loader.read("Assets/Cube.obj");

	cube = loader.object();

	angle = 15.0;
	cubeX = 0;
	cubeY = 0;

	xInc = 0.1;
	yInc = ((float) width / height) / 10;

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(45.0, ((float)width / height), 1.0, 20.0);
	glMultMatrixf(Mat4::perspectiveMatrix(45.0, ((float)width / height), 1.0, 20.0));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vec4 camera(0.0, 0.0, 1.0);
	Vec4 target(0.0, 0.0, 0.0);
	Vec4 up(0.0, 1.0, 0.0);

	Vec4 v(3, 1, 2, 0);

	cout << v << endl;
	cout << v.length() << endl;
	cout << v.normalised() << endl;

//	exit(0);

	//glMultMatrixf(Mat4::lookAt(camera, target, up));
	gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	GLfloat values[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, values);
	cout << "GOOD" << Mat4(values) << endl;
	cout << "MINE" << Mat4::lookAt(camera, target, up) << endl;
	exit(0);

	glViewport(0, 0, width, height);

}

void Core::drawCube(int i_x, int i_y)
{
	float x = centreX(cube);
	float y = centreY(cube);
	float z = centreZ(cube);

	glPushMatrix();

	float scale = 0.125;
	float normScale = (scale * 2) / scale;

	float curX = ((float) i_x / width) * normScale * 2 - normScale;
	float curY = ((float) i_y / height) * normScale * 2 - normScale;

	glMultMatrixf(Mat4::translate(curX, curY, -z));

	glMultMatrixf(Mat4::scale(scale, scale, scale));

	glMultMatrixf(Mat4::rotateX(angle));
	glMultMatrixf(Mat4::rotateY(angle));
	glMultMatrixf(Mat4::rotateZ(angle));

	glMultMatrixf(Mat4::translate(-x, -y, -z));

	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), &cube[0]);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &cube[0].r);

	glDrawArrays(GL_QUADS, 0, 4);
	glDrawArrays(GL_QUADS, 4, 8);
	glDrawArrays(GL_QUADS, 8, 12);
	glDrawArrays(GL_QUADS, 12, 16);

	glPopMatrix();
}

void Core::render()
{
	// Draw Objects here

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	drawCube(round(cubeX), round(cubeY));

	angle += elapsedTime * 100;

	cubeX += xInc;
	cubeY += yInc;

	if (cubeY >= height || cubeY < 0)
		yInc = -yInc;

	if (cubeX >= width || cubeX < 0)
		xInc = -xInc;

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// Flip the buffer for double buffering
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
