/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 */
#include "Assignment1.h"
#include "ObjectLoader.h"
#include "Transformation.h"
#include "Clipper.h"

#include <algorithm>
using std::sort;

#include <cassert>

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

	Assignment1 example;
	example.start();
	return 0;
}

Assignment1::~Assignment1()
{
	delete projection;
}

void Assignment1::preprocess()
{
	// build the cube
	ObjectLoader loader;

	loader.read("Assets/Cube.obj");

	cube.faces = loader.object();
	//mesh = loader.object();
	cube.scale = 0.125;

	cube.x = 0;
	cube.y = 0;
	angle = 15.0;
	cube.speed = 10;

	xInc = 1 / cube.speed;
	yInc = ((float) width / height) / cube.speed;

	// load identity matrix
	projection = new Mat4();

	Mat4 perspectiveMatrix = Mat4::perspectiveMatrix(45.0, ((float) width / height), 1.0, 500.0);
	(*projection) = Mat4::mul((*projection), perspectiveMatrix);
}

void Assignment1::drawCube(Cube cube)
{
	float x = cube.centreX();
	float y = cube.centreY();
	float z = cube.centreZ();

	float scale = cube.scale;
	float normScale = (scale * 2) / scale;

	float curX = (cube.x / width) * normScale * 2 - normScale;
	float curY = (cube.y / height) * normScale * 2 - normScale;

	Mat4 model;

//	model = Mat4::mul(model, Mat4::translate(curX, curY, z));
	model = model * Mat4::scale(scale, scale, scale);
/*	model = model * Mat4::rotateX(angle);
	model = model * Mat4::rotateY(angle);
	model = model * Mat4::rotateZ(angle);

	model = Mat4::mul(model, Mat4::translate(-x, -y, -z));
*/
	Vec4 camera(0.0, 0.0, 1.0);
	//Vec4 target(cube.x, cube.y, cube.y);
	Vec4 target(0, 0, 0);
	Vec4 up(0.0, -1.0, 0.0);

	Mat4 view = Mat4::lookAt(camera, target, up);

	Mat4 modelViewPerspective = model * view * (*projection);
	//Mat4 modelViewPerspective = model * (*projection);

	//for (int i=0; i < cube.faces.size(); i++)
	for (unsigned i=5; i < 6; i++)
	{
		Face currentFace = cube.faces[i];
		Face newFace;

		for (int j=0; j < currentFace.size(); j++)
		{
			Vertex v = currentFace[j];

			// get the NDC
			Vec4 vec = Mat4::mul(modelViewPerspective, Vec4(v(0), v(1), v(2), v(3)));

			cout << "before normalisation " << endl << vec << endl;

			// normalised x, y and z against w
			vec(0) /= vec(3);
			vec(1) /= vec(3);
			vec(2) /= vec(3);

			cout << "after normalisation " << endl << vec << endl;

			int x = (vec(0) + 1) * (width / 2);
			int y = height - ((vec(1) + 1) * (height / 2));

			int r = v(4);
			int g = v(5);
			int b = v(6);

			newFace.push_back(Vertex(x, y, vec(2), vec(3), r, g, b, 1));
		}

/*		cout << "start" << endl;
		for (int b=0; b < newFace.size(); b++)
			cout << newFace[b] << endl;
		cout << "end" << endl;
*/
		drawPolygon(newFace);
	}

	// multiply view (lookat) by the model to get modelview matrix
	// multiply perspective by modelview to get your model view perspective matrix

	// for loop for each face of the cube
	// store each vector as a Vec4
	// multiply modelview perspective by each vec4 for the NDC
	// divide each point in the NDC by w for homogeneous normalised points
	// use the NDC to get the actual pixels.
	// get the colours from the original shape
	// make a new vertex using the colours and x and y values.
	// add that vertex to a new face to preserve the original faces.
	// draw the polygon using the face.
}

void Assignment1::moveCube(Cube & cube)
{
	angle += elapsedTime * 100;

	if (cube.y >= height || cube.y < 0)
		yInc = -yInc;

	if (cube.x >= width || cube.x < 0)
		xInc = -xInc;

	cube.x += xInc;
	cube.y += yInc;
}

void Assignment1::drawPolygon(vector<Vertex> polygon)
{
	vector<Vertex> clipped = Clipper::clip(polygon, width, height);
	vector<Vertex> decomposed = decompose(clipped);

	if (decomposed.size() == 0)
		return;

	assert(decomposed.size() % 3 == 0);

	for (int i = 0; i < decomposed.size() -2; i+=3)
	{
		//SDL_LockSurface(buffer);
		triangle(decomposed[i], decomposed[i+1], decomposed[i+2]);
		//sleep(1);
		//SDL_UnlockSurface(buffer);
		//SDL_Flip(buffer);

	}
}

void Assignment1::triangle(Vertex a, Vertex b, Vertex c)
{
/*	Vec4 AB = Vec4(a(0), a(1), a(2), 0) * Vec4(b(0), b(1), b(2), 0);
	Vec4 BC = Vec4(b(0), b(1), b(2), 0) * Vec4(c(0), c(1), c(2), 0);

	Vec4 cross = AB * BC;
*/
//	if (((int) cross(2)) < 0)
//		return;

	cout << "drawing a triangle with these three points: " << endl;
	cout << a << endl;
	cout << b << endl;
	cout << c << endl;
	cout << "end" << endl << endl;

	// first, we sort the vertices on the Y axis, using sort from algorithm library.
	vector<Vertex> sorted;

	sorted.push_back(a);
	sorted.push_back(b);
	sorted.push_back(c);

	sort(sorted.begin(), sorted.end(), compareOnY);

	a = sorted[0];
	b = sorted[1];
	c = sorted[2];

	// we'll make all the edges now rather than littering them through code.
	vector<Vertex> ab_edge = makeLine(a, b);
	vector<Vertex> bc_edge = makeLine(b, c);
	vector<Vertex> ac_edge = makeLine(a, c);

	vector<Vertex> l_edge, r_edge;
	l_edge = ab_edge;
	r_edge = ac_edge;

	// build the left and right edges by comparing x values.
	if (b(0) < c(0))
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

	assert(l_edge.size() == r_edge.size());

	// now, let's paint it.
	for (unsigned i=0; i < l_edge.size();i++)
		scanLine(l_edge[i], r_edge[i]);
}

vector<Vertex> Assignment1::makeLine(Vertex a, Vertex b)
{
	vector<Vertex> result;

	double x = a(0);

	double red = a(4);
	double green = a(5);
	double blue = a(6);

	double dy = b(1) - a(1);
	double dx = b(0) - a(0);

	double slope = dy / dx;

	double r_inc = (double) (b(4) - a(4)) / dy;
	double g_inc = (double) (b(5) - a(5)) / dy;
	double b_inc = (double) (b(6) - a(6)) / dy;

	int max = round(b(1));

	for (int y = round(a(1)); y < max; y++)
	{
		result.push_back(Vertex(round(x), y, 0, 0, round(red), round(green), round(blue), 1));

		red += r_inc;
		green += g_inc;
		blue += b_inc;
		x += 1 / slope;
	}
	return result;
}

void Assignment1::scanLine(Vertex a, Vertex b)
{
	double red = a(4);
	double green = a(5);
	double blue = a(6);
	double dx = b(0) - a(0);

	if (a(0) > b(0))
		swap<Vertex>(a, b);

	double r_inc = (double) (b(4) - a(4)) / dx;
	double g_inc = (double) (b(5) - a(5)) / dx;
	double b_inc = (double) (b(6) - a(6)) / dx;

	int x = a(0);
	while (x < b(0))
	{
		putpixel(x, a(1), round(red), round(green), round(blue));
		x++;

		red += r_inc;
		blue += b_inc;
		green += g_inc;
	}
}

vector<Vertex> Assignment1::decompose(vector<Vertex> polygon)
{
	vector<Vertex> result;

	for (int i=1, max = polygon.size() -1; i < max; i++)
	{
		result.push_back(polygon[0]);
		result.push_back(polygon[i]);
		result.push_back(polygon[i+1]);
	}

	return result;
}

void Assignment1::render()
{
	SDL_FillRect(buffer, NULL, 0);

	SDL_LockSurface(buffer);
	drawCube(cube);
	moveCube(cube);
	SDL_UnlockSurface(buffer);
	SDL_Flip(buffer);
}
