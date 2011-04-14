/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 */
#include "Core.h"
#include "ObjectLoader.h"
#include "Primitives.h"
#include "Transformation.h"
#define round(x) (int) x+0.5

#include <algorithm>
using std::sort;

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
	Uint32 flags = SDL_HWSURFACE | SDL_DOUBLEBUF;

	if (fullscreen)
		flags |= SDL_FULLSCREEN;

	buffer = SDL_SetVideoMode(width, height, 32, flags);
	if (!buffer) {
		cerr << SDL_GetError() << endl;
		SDL_Quit();
	}

	// Initialise SDL_ttf, a truetype wrapper for SDL
	if (TTF_Init() < 0) {
		cerr << TTF_GetError() << endl;
		TTF_Quit();
		SDL_Quit();
	}

	SDL_FillRect(buffer, NULL, 0);
	SDL_Flip(buffer);

}

void Core::preprocess()
{
	// Define or load objects here
	//

	ObjectLoader loader;

	loader.read("Assets/Cube.obj");

	cube.mesh = loader.object();
	cube.scale = 0.125;

	cube.x = 0;
	cube.y = 0;
	angle = 15.0;
	cube.speed = 100;

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(Mat4::perspectiveMatrix(45.0, ((float)width / height), 1.0, 20.0));


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Vec4 camera(0, 0, 3.5), target(0, 0, 0), up(0, 1.0, 0);
	glMultMatrixf(Mat4::lookAt(camera, target, up));

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

	glMultMatrixf(Mat4::translate(-x, -y, -z));
	glMultMatrixf(Mat4::scale(scale, scale, scale));

	glMultMatrixf(Mat4::rotateX(angle));
	glMultMatrixf(Mat4::rotateY(angle));
	glMultMatrixf(Mat4::rotateZ(angle));

	glMultMatrixf(Mat4::translate(-x, -y, -z));

	/*
	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), &cube.mesh[0]);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &cube.mesh[0].r);

	glDrawArrays(GL_QUADS, 0, 4);
	glDrawArrays(GL_QUADS, 4, 8);
	glDrawArrays(GL_QUADS, 8, 12);
	glDrawArrays(GL_QUADS, 12, 16);
	*/

	GLfloat values[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, values);

	drawPolygon(cube.getPoints(0, 4));
	drawPolygon(cube.getPoints(4, 8));
	drawPolygon(cube.getPoints(8, 12));
	drawPolygon(cube.getPoints(12, 16));


	glPopMatrix();
}

void Core::render()
{
	// Draw Objects here
	SDL_LockSurface(buffer);

	drawCube(cube);

	triangle(Point(20, 250, 255), Point(200, 200, 0, 255), Point(100, 100, 0, 0, 255));
	SDL_UnlockSurface(buffer);
	SDL_Flip(buffer);
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

					case SDLK_SPACE:
						{
							int x1, y1, x2, y2, x3, y3;

							x1 = rand() % width;
							x2 = rand() % width;
							x3 = rand() % width;

							y1 = rand() % height;
							y2 = rand() % height;
							y3 = rand() % height;

							Point a = Point(x1, y1, 255);
							Point b = Point(x2, y2, 0, 255);
							Point c = Point(x3, y3, 0, 0, 255);

							SDL_LockSurface(buffer);
							triangle(a, b, c);
							SDL_UnlockSurface(buffer);
							SDL_Flip(buffer);
							break;
						}

					default: break;
				}
				break;
			default: break;
		}
	}
}

vector<Point> Core::decompose(vector<Point> polygon)
{
	vector<Point> result;

	for (unsigned i=1; i < polygon.size() -1; i++)
	{
		result.push_back(polygon[0]);
		result.push_back(polygon[i]);
		result.push_back(polygon[i+1]);
	}

	return result;
}

vector<Point> Core::clipLeft(vector<Point> polygon)
{
	vector<Point> result;

	const int minX = 0;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		// if the vertex is invisible, ignore it.
		if (a.x < minX && b.x < minX)
			continue;
		// if both are visible, add the first one.
		else if (a.x >= minX && b.x >= minX)
			result.push_back(a);
		// else, deal with a partially visible line.
		else
		{
			double dy = b.y - a.y;
			double dx = b.x - a.x;

			// if a == in and b == out
			if (a.x >= minX)
			{
				b.y += ((double)(dy / dx)) * (double)(minX - b.x);
				b.x = minX;

				result.insert(result.begin(), b);
				result.insert(result.begin(), a);
			}
			else // a == out and b == in
			{
				a.y += ((double)(dy / dx)) * (double)(minX - a.x);
				double oldX = a.x;
				a.x = minX;

				double r_inc = (double) (b.r - a.r) / dy;
				double g_inc = (double) (b.g - a.g) / dy;
				double b_inc = (double) (b.b - a.b) / dy;

				a.r += r_inc * (oldX - a.x);
				a.g += g_inc * (oldX - a.x);
				a.b += b_inc * (oldX - a.x);

				result.insert(result.begin(), a);
			}
		}

	}

	return result;
}


vector<Point> Core::clipRight(vector<Point> polygon)
{
	vector<Point> result;
	const int maxX = width;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		if (a.x > maxX && b.x > maxX)
			continue;
		else if (a.x <= maxX && b.x <= maxX)
			result.push_back(a);
		else
		{
			double dy = b.y - a.y;
			double dx = b.x - a.x;

			// if a == in and b == out
			if (a.x <= maxX)
			{
				b.y += (dy / dx) * (maxX - b.x);
				b.x = maxX-1;

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				a.y += (dy / dx) * (maxX - a.x);
				a.x = maxX-1;

				result.push_back(a);
			}
		}

	}

	return result;
}

vector<Point> Core::clipBottom(vector<Point> polygon)
{
	vector<Point> result;
	const int maxY = height;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		// if the vertex is invisible, ignore it.
		if (a.y > maxY && b.y > maxY)
			continue;
		else if (a.y <= maxY && b.y <= maxY)
			result.push_back(a);
		else
		{
			double dy = b.y - a.y;
			double dx = b.x - a.x;

			// if a == in and b == out
			if (a.y < maxY)
			{
				b.x += (maxY - b.y) / (dy / dx);
				b.y = maxY-1;

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				a.x += (maxY - a.y) / (dy / dx);
				a.y = maxY-1;

				result.push_back(a);
			}
		}

	}

	return result;
}

vector<Point> Core::clipTop(vector<Point> polygon)
{
	vector<Point> result;
	const int minY = 0;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		// if the vertex is invisible, ignore it.
		if (a.y < minY && b.y < minY)
			continue;
		else if (a.y >= minY && b.y >= minY)
			result.push_back(a);
		else
		{
			double dy = b.y - a.y;
			double dx = b.x - a.x;

			// if a == in and b == out
			if (a.y >= minY)
			{
				b.x += (minY - b.y) / (dy / dx);
				b.y = minY;

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				a.x += (minY - a.y) / (dy / dx);
				a.y = minY;

				result.push_back(a);
			}
		}

	}

	return result;
}

vector<Point> Core::clip(vector<Point> polygon)
{
	polygon = clipTop(polygon);
	polygon = clipLeft(polygon);
	polygon = clipRight(polygon);
	polygon = clipBottom(polygon);

	return polygon;
}

void Core::drawPolygon(vector<Point> polygon)
{

	vector<Point> clipped = clip(polygon);
	vector<Point> decomposed = decompose(clipped);

	for (unsigned i = 0; i < decomposed.size() -2; i+=3)
		triangle(decomposed[i], decomposed[i+1], decomposed[i+2]);
}

void Core::triangle(Point a, Point b, Point c)
{
	// first, we sort the vertices on the Y axis, using sort from algorithm library.
	vector<Point> sorted;

	sorted.push_back(a);
	sorted.push_back(b);
	sorted.push_back(c);

	sort(sorted.begin(), sorted.end(), compareOnY);

	a = sorted[0];
	b = sorted[1];
	c = sorted[2];

	// we'll make all the edges now rather than littering them through code.
	vector<Point> ab_edge = makeLine(a, b);
	vector<Point> bc_edge = makeLine(b, c);
	vector<Point> ac_edge = makeLine(a, c);

	vector<Point> l_edge, r_edge;
	l_edge = ab_edge;
	r_edge = ac_edge;

	// find the left and right edges by comparing x values.
	if (b.x < c.x)
	{

		if (l_edge.size() < r_edge.size())
		{
			for (unsigned i = 0; i < bc_edge.size(); i++)
				l_edge.push_back(bc_edge[i]);
		}
		else
		{
			for (unsigned i = 0; i < bc_edge.size(); i++)
				r_edge.push_back(bc_edge[i]);
		}
	}
	else
	{
		swap(l_edge, r_edge);

		if (l_edge.size() < r_edge.size())
		{
			for (unsigned i = 0; i < bc_edge.size(); i++)
				l_edge.push_back(bc_edge[i]);
		}
		else
		{
			for (unsigned i = 0; i < bc_edge.size(); i++)
				r_edge.push_back(bc_edge[i]);
		}
	}

	// now, let's paint it.
	for (unsigned i=0; i < l_edge.size();i++)
		scanLine(l_edge[i], r_edge[i]);
}

vector<Point> Core::makeLine(Point a, Point b)
{
	vector<Point> result;

	// we want to x as a float or it won't work!
	double x = a.x;

	// store colours for same reason
	double red = a.r;
	double green = a.g;
	double blue = a.b;

	double dy = b.y - a.y;
	double dx = b.x - a.x;

	double slope = dy / dx;

	double r_inc = (double) (b.r - a.r) / dy;
	double g_inc = (double) (b.g - a.g) / dy;
	double b_inc = (double) (b.b - a.b) / dy;

	for (; a.y < b.y; a.y++)
	{
		result.push_back(Point(round(x), a.y, round(red), round(green), round(blue)));

		red += r_inc;
		green += g_inc;
		blue += b_inc;
		x += 1 / slope;
	}
	return result;
}

void Core::scanLine(Point a, Point b)
{
	double red = a.r;
	double green = a.g;
	double blue = a.b;
	double dx = b.x - a.x;

	if (a.x > b.x)
		swap<Point>(a, b);

	double r_inc = (double) (b.r - a.r) / dx;
	double g_inc = (double) (b.g - a.g) / dx;
	double b_inc = (double) (b.b - a.b) / dx;

	while (a.x <= b.x)
	{
		putpixel(a.x, a.y, round(red), round(green), round(blue));
		a.x++;

		red += r_inc;
		blue += b_inc;
		green += g_inc;
	}
}

void Core::putpixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Uint8 *p = (Uint8 *)buffer->pixels + y * buffer->pitch + x * 4;

#if defined(__MACH__) && defined(__APPLE__)
	*(Uint32 *)p = b << 24 | g << 16 | r << 8 | a; // Big endian
#else
	*(Uint32 *)p = b | g << 8 | r << 16 | a << 24; // Lil endian
#endif
}
