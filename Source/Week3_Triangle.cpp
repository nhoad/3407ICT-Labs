//////////////////////////////////////////////////////////////
/// Author:             <Your Name>
/// Student Number:     <s0000000>
/// Operating System:   <Windows|Mac OS X|Linux>
/// Comments:           <Notes about running your program>
//////////////////////////////////////////////////////////////

#include "Week3_Triangle.h"
#include <iostream>
#include <algorithm>
#define round(x) (int) x + 0.5
using namespace std;

#if defined(__MACH__) && defined(__APPLE__)
// Allow SDL main hack, because of the OS X Cocoa binding
#else
// SDL main hackery disabled for Windows and Linux
#define SDL_main main
#endif

int main(int argc, char* argv[])
{

	srand(time(NULL));
	Core example;
	example.start();

	return 0;
}

int compareOnX(Point a, Point b)
{
	return (a.x < b.x);
}

int compareOnY(Point a, Point b)
{
	return (a.y < b.y);
}


Core::Core(int width, int height) : running(true), point(0, 0, 255)
{
	this->width = width;
	this->height = height;
}

Core::~Core()
{
	// Allows SDL to exit properly
	if (SDL_WasInit(SDL_INIT_VIDEO))
		SDL_Quit();
}

void Core::start()
{
	// Setup and then enter main loop
	initialise();
	while (running) {
		handleEvents();
	}
}

void Core::initialise()
{
	// Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cerr << SDL_GetError() << endl;
		SDL_Quit();
	}

	// Create a framebuffer
	Uint32 flags = SDL_SWSURFACE | SDL_DOUBLEBUF;

	buffer = SDL_SetVideoMode(width, height, 32, flags);
	if (!buffer) {
		cerr << SDL_GetError() << endl;
		SDL_Quit();
	}

	// Clear the screen to black
	SDL_FillRect(buffer, NULL, 0);
	SDL_Flip(buffer);
}

void Core::draw(Point a, Point b)
{
	SDL_LockSurface(buffer);
	/////////////////////////////////////
	// Draw objects here
	line(a, b);

	/////////////////////////////////////
	SDL_UnlockSurface(buffer);
	// Flip the buffers
	SDL_Flip(buffer);
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

	while (a.x < b.x)
	{
		putpixel(a.x, a.y, round(red), round(green), round(blue));
		a.x++;

		red += r_inc;
		blue += b_inc;
		green += g_inc;
	}
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

void Core::line(Point a, Point b)
{
	vector<Point> result;

	if (a.x > b.x)
		swap(a, b);

	// we want to store x and y as floats or it won't work!
	double y = a.y;
	double x = a.x;

	// store colours for same reason
	double red = a.r;
	double green = a.g;
	double blue = a.b;

	double dy = b.y - a.y;
	double dx = b.x - a.x;

	double slope = dy / dx;

	if (slope <= 1.0 && slope > -1.0)
	{
		double r_inc = (double) (b.r - a.r) / dx;
		double g_inc = (double) (b.g - a.g) / dx;
		double b_inc = (double) (b.b - a.b) / dx;

		for (; a.x < b.x; a.x++)
		{
			putpixel(a.x, round(y), round(red), round(green), round(blue));

			red += r_inc;
			green += g_inc;
			blue += b_inc;
			y += slope;
		}

		return;
	}
	else if (slope <= -1.0)
	{
		// because we're drawing a steep negative slope, swap a and b back to their original positions.
		swap(a, b);

		x = a.x;
		dy = b.y - a.y;
	}

	double r_inc = (double) (b.r - a.r) / dy;
	double g_inc = (double) (b.g - a.g) / dy;
	double b_inc = (double) (b.b - a.b) / dy;

	for (; a.y < b.y; a.y++)
	{
		putpixel(round(x), a.y, round(red), round(green), round(blue));

		red += r_inc;
		green += g_inc;
		blue += b_inc;
		x += 1 / slope;
	}

	////////////////////////////////////////////
}

vector<Point> Core::decompose(vector<Point> polygon)
{
	vector<Point> result;

	// build triangles from the first point, next point and final point.
	while (polygon.size() > 2)
	{
		result.push_back(polygon[0]);
		result.push_back(polygon[1]);
		result.push_back(polygon[polygon.size()-1]);

		polygon.erase(polygon.begin(), polygon.begin()+1);
	}

	return result;
}

vector<Point> Core::clip_left(vector<Point> polygon)
{
	vector<Point> result;

	const int minX = 0;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		if (a.x < minX && b.x < minX)
			continue;
		else if (a.x >= minX && b.x >= minX)
			result.push_back(a);
		else
		{
			bool swapped = false;

			// if a.x is within bounds, operate on b.
			if (a.x >= minX)
			{
				swapped = true;
				swap(a, b);
			}

			double dy = b.y - a.y;
			double dx = b.x - a.x;

			a.y += ((double)(dy / dx)) * (double)(minX - a.x);
			a.x = minX;

			result.insert(result.begin(), a);
			if (swapped)
				result.push_back(b);
		}

	}

	return result;
}

vector<Point> Core::clip_right(vector<Point> polygon)
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
			bool swapped = false;

			// if a.x is within boundaries, we'll operate on b.
			if (a.x <= maxX)
			{
				swapped = true;
				swap(a, b);
			}

			double dy = b.y - a.y;
			double dx = b.x - a.x;

			a.y += (dy / dx) * (maxX - a.x);
			a.x = maxX-1;

			if (swapped)
				result.push_back(b);

			result.push_back(a);
		}

	}

	return result;
}

vector<Point> Core::clip_bottom(vector<Point> polygon)
{
	vector<Point> result;
	const int maxY = height;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		if (a.y > maxY && b.y > maxY)
			continue;
		else if (a.y <= maxY && b.y <= maxY)
			result.push_back(a);
		else
		{
			bool swapped = false;

			double dy =(double) b.y / a.y;
			double dx =(double) b.x / a.x;

			// if a.y is within boundaries, we'll operate on b.
			if (a.y <= maxY)
			{
				swapped = true;
				swap(a, b);
			}

			a.x += (maxY - a.y) / (dy / dx);
			a.y = maxY-1;

			if (swapped)
				result.push_back(b);

			result.push_back(a);
		}

	}

	return result;
}
vector<Point> Core::clip_top(vector<Point> polygon)
{
	vector<Point> result;
	const int minY = 0;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		if (a.y < minY && b.y < minY)
			continue;
		else if (a.y >= minY && b.y >= minY)
			result.push_back(a);
		else
		{
			bool swapped = false;

			// if a.y is within boundaries, we'll operate on b.
			if (a.y >= minY)
			{
				swapped = true;
				swap(a, b);
			}

			double dy = (double) b.y / a.y;
			double dx = (double) b.x / a.x;

			a.x += (minY - a.y) / (dy / dx);
			a.y = minY;

			result.insert(result.begin(), a);
			if (swapped)
				result.push_back(b);
		}

	}

	return result;
}
vector<Point> Core::clip(vector<Point> polygon)
{
	cout << endl << "before clipping: " << endl;
	for (unsigned i=0; i < polygon.size(); i++)
		cout << i << " : " << polygon[i].x << ' ' << polygon[i].y << endl;

	polygon = clip_bottom(polygon);
	polygon = clip_top(polygon);
	polygon = clip_left(polygon);
	polygon = clip_right(polygon);

	cout << endl << "after clipping: " << endl;
	for (unsigned i=0; i < polygon.size(); i++)
		cout << i << " : " << polygon[i].x << ' ' << polygon[i].y << endl;

	return polygon;
}

void Core::draw_polygon(vector<Point> polygon)
{
	vector<Point> clipped = clip(polygon);
	vector<Point> decomposed = decompose(clipped);

	for (unsigned i = 0; i < decomposed.size() -2; i+=3)
	{
		SDL_LockSurface(buffer);
		triangle(decomposed[i], decomposed[i+1], decomposed[i+2]);
		SDL_UnlockSurface(buffer);
		SDL_Flip(buffer);
		sleep(1);
	}
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

	// find the left and right edges by comparing x values.
	if (b.x < c.x)
	{
		l_edge = ab_edge;
		r_edge = ac_edge;

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
	else if (c.x < b.x)
	{
		l_edge = ac_edge;
		r_edge = ab_edge;

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
		l_edge = ac_edge;
		r_edge = bc_edge;

		if (l_edge.size() < r_edge.size())
		{
			for (unsigned i = 0; i < bc_edge.size(); i++)
				l_edge.push_back(bc_edge[i]);
		}
		else
		{
			for (unsigned i = 0; i < ab_edge.size(); i++)
				r_edge.push_back(ab_edge[i]);
		}
	}

	// now, let's paint it.
	for (unsigned i=0; i < l_edge.size();i++)
		scanLine(l_edge[i], r_edge[i]);
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
					case SDLK_RETURN:
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
					case SDLK_SPACE:
						SDL_LockSurface(buffer);
						triangle(Point(50, 200, 255),
									Point(200, 200, 0, 255),
									Point(125, 50, 0,0,255));

						triangle(Point(250, 200, 255),
									Point(400, 200, 0, 255),
									Point(250, 50, 0,0,255));

						triangle(Point(80, 10, 255),
									Point(70, 30, 0, 255),
									Point(100, 60, 0, 0, 255));

						triangle(Point(350, 200, 255),
									Point(550, 250, 0, 255),
									Point(650, 500, 0, 0, 255));

						triangle(Point(50, 250, 255),
									Point(250, 250, 0, 255),
									Point(250, 500, 0, 0, 255));

						SDL_UnlockSurface(buffer);
						SDL_Flip(buffer);
						break;
					case SDLK_1:
					{
						vector<Point> polygon;

						// below is a nice normal polygon
						polygon.push_back(Point(100, 200, 255));
						polygon.push_back(Point(150, 100, 0, 255));
						polygon.push_back(Point(300, 100, 0, 0, 255));
						polygon.push_back(Point(400, 300, 255, 255));
						polygon.push_back(Point(200, 500, 255, 0, 255));

						draw_polygon(polygon);
						break;
					}
					case SDLK_2:
					{
						vector<Point> polygon;

						// this polygon will be clipped before it's drawn
						polygon.push_back(Point(-100, 300, 255)); // clipped on left
						polygon.push_back(Point(400, -100, 0, 255)); // clipped on top
						polygon.push_back(Point(900, 300, 0, 0, 255)); // clipped on right
						polygon.push_back(Point(400, 700, 255, 255)); // clip on bottom

						/*polygon.push_back(Point(100, 300, 255));
						polygon.push_back(Point(400, 100, 0, 255));
						polygon.push_back(Point(700, 300, 0, 0, 255));
						polygon.push_back(Point(400, 700, 255, 255));
						*/draw_polygon(polygon);
						break;
					}

					default: break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				draw(point, Point(e.button.x, e.button.y, 0,0, 255));
				point.x = e.button.x;
				point.y = e.button.y;
				point.b = point.r;
				point.r = point.g;
				point.g = point.b;
				break;
			default: break;
		}
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
