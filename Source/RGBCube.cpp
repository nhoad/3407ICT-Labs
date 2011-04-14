/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 */
#include "RGBCube.h"
#include "ObjectLoader.h"
#include "Transformation.h"
#define round(x) (int)( x + 0.5)

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

	cube.mesh = loader.object();
	//mesh = loader.object();
	cube.scale = 0.125;

	cube.x = 0;
	cube.y = 0;
	angle = 15.0;
	cube.speed = 10;

	xInc = 1 / cube.speed;
	yInc = ((float) width / height) / cube.speed;

	glEnable(GL_DEPTH_TEST);

	// load identity matrix
	projection = new Mat4();

	Mat4 perspectiveMatrix = Mat4::perspectiveMatrix(45.0, ((float) width / height), 1.0, 20.0);
	(*projection) = Mat4::mul((*projection), perspectiveMatrix);
	//gluPerspective(45.0, ((float)width / height), 1.0, 20.0);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projection);

	modelview = new Mat4();

	Vec4 camera(0.0, 0.0, 3.5);
	Vec4 target(0.0, 0.0, 0.0);
	Vec4 up(0.0, 1.0, 0.0);

	(*modelview) = Mat4::mul((*modelview), Mat4::lookAt(camera, target, up));
	//gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*modelview);
	glViewport(0, 0, width, height);

}

void Core::drawCube(Cube cube)
{

	float x = cube.centreX();
	float y = cube.centreY();
	float z = cube.centreZ();

	float scale = cube.scale;
	float normScale = (scale * 2) / scale;

	float curX = (cube.x / width) * normScale * 2 - normScale;
	float curY = (cube.y / height) * normScale * 2 - normScale;

	glPushMatrix();

	glMultMatrixf(Mat4::translate(curX, curY, -z));

	glMultMatrixf(Mat4::scale(scale, scale, scale));

	glMultMatrixf(Mat4::rotateX(angle));
	glMultMatrixf(Mat4::rotateY(angle));
	glMultMatrixf(Mat4::rotateZ(angle));

	glMultMatrixf(Mat4::translate(-x, -y, -z));

	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), &cube.mesh[0]);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &cube.mesh[0].r);


/*
	GLfloat values[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, values);
	cout << Mat4(values) << endl;
*/

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

	drawCube(cube);

	angle += elapsedTime * 100;

	if (cube.y >= height || cube.y < 0)
		yInc = -yInc;

	if (cube.x >= width || cube.x < 0)
		xInc = -xInc;

	cube.x += xInc;
	cube.y += yInc;

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
