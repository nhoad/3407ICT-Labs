/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 */
#include "Assignment1.h"
#include "ObjectLoader.h"
#include "Transformation.h"
#include "Clipper.h"
#include "StringFunctions.h"

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

	if (argc < 2)
	{
		example.addModelFile("Assets/Cube.obj");
	}
	else
	{
		for (int i=1; i < argc; i++)
			example.addModelFile(argv[i]);
	}

	example.start();
	return 0;
}

Assignment1::~Assignment1()
{
	delete projection;
	delete zBuffer;
}

void Assignment1::addModelFile(string filename)
{
	modelFile = filename;

	ObjectLoader loader;
	Object o;

	o.faces = loader.read(filename, true);

	objects.push_back(o);
}

void Assignment1::preprocess()
{
	for (int i=0; i < objects.size(); i++)
	{
		objects[i].x = 50 + (i * 300);
		objects[i].y = height / 2;
	}

	if (objects.size() == 1)
		objects[0].x = width / 2;

	speed = 0.1;
	scale = 0.5;
	xInc = 0;
	yInc = 0;
	angleX = 0.0;
	angleY = 0.0;
	angleZ = 0.0;

	// load identity matrix
	projection = new Mat4();

	Mat4 perspectiveMatrix = Mat4::perspectiveMatrix(45.0, ((float) width / height), 10.0, 500.0);
	(*projection) = *projection * perspectiveMatrix;

	increaseScale = false;
	decreaseScale = false;
	dynamic = false;

	rotateX = false;
	rotateY = false;
	rotateZ = false;

	wireframe = true;

	font = TTF_OpenFont("Assets/calibri.ttf", 12);

	if (!font)
		cerr << "could not load font!" << endl;

	zBuffer = new float[width * height];

	for (int i=0, max = width * height; i < max; i++)
		zBuffer[i] = 500;

	loadInstructions();
}

void Assignment1::line(Vertex a, Vertex b)
{
	if (a(0) > b(0))
		swap(a, b);

	double slope = (b(1) - a(1)) / (b(0) - a(0));

	if (slope <= 1.0 && slope > -1.0)
	{
		float y = a(1);
		for (int x = round(a(0)), max = round(b(0)); x < max; x++)
		{
			putpixel(x, round(y), 255, 255, 255);
			y += slope;
		}
		return;
	}
	else if (slope <= -1.0)
		swap(a, b);

	float x = a(0);
	for (int y = round(a(1)), max = round(b(1)); y < max; y++)
	{
		putpixel(round(x), y, 255, 255, 255);
		x += 1 / slope;
	}

}

void Assignment1::drawObject(Object obj)
{
	float x = obj.centreX();
	float y = obj.centreY();
	float z = obj.centreZ();

	float normScale = (scale * 2) / scale;

	float curX = (obj.x / width) * normScale * 2 - normScale;
	float curY = (obj.y / height) * normScale * 2 - normScale;

	Mat4 model;

	model *= Mat4::rotateX(angleX);
	model *= Mat4::rotateY(angleY);
	model *= Mat4::rotateZ(angleZ);

	model = Mat4::translate(-x, -y, -z) * model;

	model *= Mat4::translate(curX, curY, z);

	model *= Mat4::scale(scale, scale, scale);

	Vec4 camera(0.0, 0.0, 1.0);
	Vec4 target(0, 0, 0);
	Vec4 up(0.0, -1.0, 0.0);

	Mat4 view = Mat4::lookAt(camera, target, up);

	Mat4 modelViewPerspective = model * view * (*projection);

	//for (int i=4; i < 5; i++)
	for (int i=0; i < obj.faces.size(); i++)
	{
		Face currentFace = obj.faces[i];
		Face newFace;

		for (int j=0; j < currentFace.size(); j++)
		{
			Vertex v = currentFace[j];

			Vec4 tmp(v(0), v(1), v(2), v(3));

			// get the NDC
			Vec4 vec = Mat4::mul(modelViewPerspective, tmp);

			// normalised x, y and z against w
			vec(0) /= vec(3);
			vec(1) /= vec(3);
			vec(2) /= vec(3);

			// work out actual screen coordinates
			int x = (vec(0) + 1) * (width / 2);
			int y = height - ((vec(1) + 1) * (height / 2));

			int r = v(4);
			int g = v(5);
			int b = v(6);

			newFace.push_back(Vertex(x, y, vec(2), vec(3), r, g, b, 1));
		}

		drawPolygon(newFace);
	}
}

void Assignment1::moveObject(Object & obj)
{
	obj.x += xInc;
	obj.y += yInc;

	if (dynamic)
	{
		if (increaseScale)
			scale += 0.01;
		else
			scale -= 0.01;

		if (scale >= 1)
		{
			increaseScale = false;
			decreaseScale = true;
		}
		else if (scale <= 0.1)
		{
			increaseScale = true;
			decreaseScale = false;
		}

		angleX += speed;
		angleY += speed;
		angleZ += speed;
	}

	else if (increaseScale)
		scale += (scale <= 0.1) ? 0.001 : 0.01;
	else if (decreaseScale)
		scale -= 0.01;

	if (scale >= 1.0)
		scale = 1.0;

	if (rotateX)
		angleX += speed;

	if (rotateY)
		angleY += speed;

	if (rotateZ)
		angleZ += speed;
}

void Assignment1::drawPolygon(vector<Vertex> polygon)
{
	vector<Vertex> clipped = Clipper::clip(polygon, width, height);
	vector<Vertex> decomposed = decompose(clipped);

	if (decomposed.size() == 0)
		return;

	assert(decomposed.size() % 3 == 0);

	for (int i = 0; i < decomposed.size() -2; i+=3)
		triangle(decomposed[i], decomposed[i+1], decomposed[i+2]);
}

void Assignment1::triangle(Vertex a, Vertex b, Vertex c)
{
	float x0=a(0), x1=b(0), x2=c(0);
	float y0=a(1), y1=b(1), y2=c(1);

	if (wireframe)
	{
		line(a, b);
		line(b, c);
		line(a, c);
		return;
	}

	int z = round((x1-x0)*(y2-y0)-(y1-y0)*(x2-x0));

	if (z >= 0)
		return;

	// first, we sort the vertices on the Y axis, using sort from algorithm library.
	vector<Vertex> sorted;

	sorted.push_back(a);
	sorted.push_back(b);
	sorted.push_back(c);

	sort(sorted.begin(), sorted.end(), compareOnY);

	a = sorted[0];
	b = sorted[1];
	c = sorted[2];

	vector<Vertex> l_edge = makeLine(a, b);
	vector<Vertex> r_edge = makeLine(a, c);
	vector<Vertex> bc_edge = makeLine(b, c);

	if (b(0) == c(0))
		swap(l_edge, r_edge);

	if (l_edge.size() < r_edge.size())
	{
		for (int i = 0; i < bc_edge.size(); i++)
			l_edge.push_back(bc_edge[i]);
	}
	else
	{
		for (int i = 0; i < bc_edge.size(); i++)
			r_edge.push_back(bc_edge[i]);
	}

	for (unsigned i=0; i < l_edge.size();i++)
		scanLine(l_edge[i], r_edge[i]);
}

void Assignment1::colourSwap(vector<Vertex> & a, vector<Vertex> & b)
{
	for (int i=0; i < a.size(); i++)
	{
		float tmp = a[i](4);
		a[i](4) = b[i](4);
		b[i](4) = tmp;

		tmp = a[i](5);
		a[i](5) = b[i](5);
		b[i](5) = tmp;

		tmp = a[i](6);
		a[i](6) = b[i](6);
		b[i](6) = tmp;

	}
}

vector<Vertex> Assignment1::makeLine(Vertex a, Vertex b)
{
	vector<Vertex> result;

	float x = a(0);

	float red = a(4);
	float green = a(5);
	float blue = a(6);

	float dy = b(1) - a(1);
	float dx = b(0) - a(0);

	float slope = dy / dx;

	float r_inc = (b(4) - a(4)) / dy;
	float g_inc = (b(5) - a(5)) / dy;
	float b_inc = (b(6) - a(6)) / dy;

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
	float a_x = a(0);
	float b_x = b(0);

	float red = a(4);
	float green = a(5);
	float blue = a(6);
	float dx = b_x - a_x;

	float r_inc = (b(4) - a(4)) / dx;
	float g_inc = (b(5) - a(5)) / dx;
	float b_inc = (b(6) - a(6)) / dx;

	int x = round(a_x);
	int y = round(a(1));

	if (a_x < b_x)
		while (x < b_x)
		{
			float z = a(2);
			int pos = x + (y * width);

			if (z < zBuffer[pos])
			{
				zBuffer[pos] = z;
				putpixel(x, y, round(red), round(green), round(blue));
			}
			x++;
			red += r_inc;
			blue += b_inc;
			green += g_inc;
		}
	else
		while (b_x <= x)
		{
			float z = a(2);
			int pos = x + (y * width);

			if (z < zBuffer[pos])
			{
				zBuffer[pos] = z;
				putpixel(x, y, round(red), round(green), round(blue));
			}
			x--;
			red -= r_inc;
			blue -= b_inc;
			green -= g_inc;
		}
}

void Assignment1::handleEvents()
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			running = false;
			return;
		}

		switch (e.key.keysym.sym)
		{
			case SDLK_DOWN:
			{
				if (e.type == SDL_KEYUP)
					yInc = 0;
				else
					yInc = 1;
				break;
			}
			case SDLK_UP:
			{
				if (e.type == SDL_KEYUP)
					yInc = 0;
				else
					yInc = -1;
				break;
			}
			case SDLK_RIGHT:
			{
				if (e.type == SDL_KEYUP)
					xInc = 0;
				else
					xInc = 1;
				break;
			}
			case SDLK_LEFT:
			{
				if (e.type == SDL_KEYUP)
					xInc = 0;
				else
					xInc = -1;
				break;
			}
			break;
			case SDLK_SPACE:
				if (e.type == SDL_KEYUP)
				{
					if (dynamic)
					{
						rotateX = false;
						rotateY = false;
						rotateZ = false;
						dynamic = false;
						decreaseScale = false;
						increaseScale = false;
					}
					else
					{
						rotateX = true;
						rotateY = true;
						rotateZ = true;
						dynamic = true;
						decreaseScale = true;
					}
				}
				break;
			case SDLK_ESCAPE:
				running = false;
				break;
			case SDLK_KP4:
				speed -= 0.05;
				break;
			case SDLK_KP5:
				angleX = 0;
				angleY = 0;
				angleZ = 0;
				rotateX = false;
				rotateY = false;
				rotateZ = false;
				dynamic = false;
				scale = 0.5;
				decreaseScale = false;
				increaseScale = false;
				break;
			case SDLK_KP6:
				speed += 0.05;
				break;
			case SDLK_KP7:
				rotateX = !rotateX;
				break;
			case SDLK_KP8:
				rotateY = !rotateY;
				break;
			case SDLK_KP9:
				rotateZ = !rotateZ;
				break;
			case SDLK_KP0:
				if (e.type == SDL_KEYUP)
					wireframe = !wireframe;
				break;
			case SDLK_KP_MINUS:
			{
				switch (e.type)
				{
					case SDL_KEYUP:
						decreaseScale = false;
						break;
					case SDL_KEYDOWN:
						decreaseScale = true;
					default:
						break;
				}
				break;
			}
			case SDLK_KP_PLUS:
			{
				switch (e.type)
				{
					case SDL_KEYUP:
						increaseScale = false;
						break;
					case SDL_KEYDOWN:
						increaseScale = true;
					default:
						break;
				}
			}
			default:
				break;

		}
	}


}

void Assignment1::drawText(const char * text, int x, int y)
{
	SDL_Color color = {255, 255, 255, 255};
	SDL_Surface * text_buffer = TTF_RenderText_Solid(font, text, color);

	if (text_buffer)
	{
		SDL_Rect pos = {x, y, 0, 0};
		SDL_BlitSurface(text_buffer, 0, buffer, &pos);
		SDL_FreeSurface(text_buffer);
	}
	else
	{
		cerr << text << " failed to render." << endl;
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

void Assignment1::loadInstructions()
{
	instructions.push_back("Spacebar: Enable/Disable Magic mode!");
	instructions.push_back("KeyPad 4: Decrease Speed");
	instructions.push_back("KeyPad 5: Reset to defaults.");
	instructions.push_back("KeyPad 6: Increase Speed");
	instructions.push_back("KeyPad 7: Rotation on X axis");
	instructions.push_back("KeyPad 8: Rotation on Y axis");
	instructions.push_back("KeyPad 9: Rotation on Z axis");
	instructions.push_back("KeyPad 0: Enable/disable wireframe");
	instructions.push_back("KeyPad + or -: Increase/Decrease Scale");
	instructions.push_back("Use the directional keys to move too!");

}

void Assignment1::render()
{
	SDL_FillRect(buffer, NULL, 0);

	SDL_LockSurface(buffer);

	for (int i=0; i < objects.size(); i++)
	{
		drawObject(objects[i]);
		moveObject(objects[i]);
	}

	SDL_UnlockSurface(buffer);

	drawText("Instructions:", 10, 10);

	for (int i=0; i < instructions.size(); i++)
		drawText(instructions[i].c_str(), 20, (i+1) * 15 + 10);

	drawText("Stats", 10, 175);

	string speedDisplay = "Rotation speed: " + typeToString<float>(speed);
	string posDisplay = "X/Y: " + typeToString<float>(objects[0].x) + " " + typeToString<float>(objects[0].y);
	string scaleDisplay = "Scale: " + typeToString<float>(scale);
	string fps = "FPS: " + typeToString<double>(1.0 / elapsedTime);
	string modelName = "Model: " + modelFile;
	//string polyCount = "Polygon Count: " + typeToString<int>(obj.faces.size());
	string wireframeText = "Wireframe: ";
	wireframeText += (wireframe) ? "enabled" : "disabled";

	vector<string> stats;

	stats.push_back(speedDisplay);
	stats.push_back(posDisplay);
	stats.push_back(scaleDisplay);
	stats.push_back(fps);
	stats.push_back(modelName);
	//stats.push_back(polyCount);
	stats.push_back(wireframeText);

	for (int i=0; i < stats.size(); i++)
		drawText(stats[i].c_str(), 20, (i+1) * 15 + 175);


	SDL_Flip(buffer);

	// reset the z buffer so this frame doesn't squash the next.
	for (int i=0, max = width * height; i < max; i++)
		zBuffer[i] = 500;
}
