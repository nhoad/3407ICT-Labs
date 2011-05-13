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

	//SDL_WM_GrabInput(SDL_GRAB_ON);
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
		elapsedTime = t.getSeconds();
		t.start();
	}
	cleanup();
}

void Core::preprocess()
{

	Object * o = new Object;

	o->mesh = Loader::readMesh("Assets/Cube.obj");

	this->objects.push_back(o);

	// Load objects here
	camera.setSpeed(0.5);
	camera.setPosition(Vec4(0, 2, 3, 1));
	camera.setTarget(Vec4(0, 0, 0, 1));

	// Add all renderable mesh to the list of objects (see header file)
	// Optional but helpful //

	// Define your projection matrix here, and your initial camera view.
	// Feel free to either use the built-in OpenGL transforms or your own ;]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ((float) width / height), 1, 2000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0, 2, 3, 0, 0, 0, 0, 1, 0);

	// Enable any OpenGL feature you like, such as backface culling and depth testing, here.
	glEnable(GL_DEPTH_TEST);

	vShader = Loader::loadShader("Assets/basic_shader_v.glsl", GL_VERTEX_SHADER);
	fShader = Loader::loadShader("Assets/basic_shader_f.glsl", GL_FRAGMENT_SHADER);

	shaderProgram = Loader::linkShader(vShader, fShader);

	//createVBOs();
}

void Core::createVBOs()
{
	for (int i=0; i < objects.size(); i++)
	{
		glGenBuffers(1, &objects[i]->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, objects[i]->vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * objects[i]->mesh.size(),
					&objects[i]->mesh[0].x, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

}

void Core::render()
{
	// Clear the frame buffer each time we draw.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable vertex array and such
	glEnableClientState(GL_VERTEX_ARRAY);

	// Push a new matrix to the GL_MODELVIEW stack.
	glPushMatrix();

	camera.load();

	// Multiply the matrix by the object's transformation matrix.

	for (int i=0; i < objects.size(); i++)
	{
		glPushMatrix();
		glMultMatrixf(objects[i]->matrix);

		glBindBuffer(GL_ARRAY_BUFFER, objects[i]->vbo);
		glVertexPointer(4, GL_FLOAT, sizeof(Vertex), &objects[i]->mesh[0]);

		glUseProgram(shaderProgram);
		glDrawArrays(GL_QUADS, 0, 24);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glPopMatrix();

	}

	// Pop the matrix from the stack.
	glPopMatrix();

	//glDisableClientState(GL_VERTEX_ARRAY);

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
						camera.look(DOWN);
						break;
					case SDLK_f:
						camera.look(UP);
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
				camera.handleMouse(e.motion.xrel, e.motion.yrel);
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
