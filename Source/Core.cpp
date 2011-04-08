/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Xavier Ho (contact@xavierho.com)
 */
#include "Core.h"
#include "ObjectLoader.h"
#include "Primitives.h"
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
	//

	ObjectLoader loader;
	loader.read("Assets/Cube.obj");
	cube = loader.object();

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMultMatrixf(Mat4::perspectiveMatrix(45.0, width / height, 1, 20).data);
//	gluPerspective(45.0, width / height, 1.0, 20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-1, -1, -1, 0, 0, 0, 0, 1, 0);

	glViewport(0, 0, width, height);

	angle = 15.0;
}

//void Core::drawCube(Mesh & mesh, int i_x, int i_y)
void Core::drawCube(Mesh & mesh, int i)
{
	float x = centreX(mesh);
	float y = centreY(mesh);
	float z = centreZ(mesh);

/*	int i_x = (i * width) / 10 + 5, i_y = 1;

	float x_offset = ((float)i_x / width) * 2 - 1;
	float y_offset = ((float)i_y / height) * 2 - 1;

	cout << "Draw at: " << x_offset << ' ' << y_offset << endl;
	cout << x_offset << endl;
*/
	glPushMatrix();

//	glMultMatrixf(Mat4::translate(x, y, 0).data);
	glMultMatrixf(Mat4::scale(0.5, 0.5, 0.5).data);

	glMultMatrixf(Mat4::rotateX(angle).data);

	glMultMatrixf(Mat4::translate(-x, -y, -z).data);

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

	for (int i=0; i < 1; i++)
	{
		cout << "drawing cube " << i << " at " << (i * width) / 10 + 5 << endl;
		//drawCube(cube, (i * width) / 10 + 5, 20);
		drawCube(cube, i);
	}

	angle += elapsedTime * 100;

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
