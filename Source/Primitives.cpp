/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#include "glew.h"

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include "Primitives.h"
#include "Loader.h"
#include "Vec3.h"

#include <iostream>
using std::ostream;

using std::string;

int compareOnY(Vertex a, Vertex b)
{
	return (a.y < b.y);
}

ostream & operator<<(ostream & o, const Vertex & v)
{
	for (int i=0; i < 4; i++)
	{
		o.width(10);
		o << v.pos[i];
	}

	for (int i=0; i < 3; i++)
	{
		o.width(10);
		o << v.normal[i];
	}

	for (int i=0; i < 2; i++)
	{
		o.width(10);
		o << v.tex[i];
	}

	return o;
}

float Terrain::getHeight(std::vector<float> * heights, int x, int y, int y_step)
{
	return (*heights)[x + (y * y_step)];
}

Vec3 Terrain::getColour(std::vector<float> * heights, int x, int y, int y_step)
{
	float height = Terrain::getHeight(heights, x, y, y_step);

	if (height == 0)
		return Vec3();

	return Vec3(0.13, 0.24, (height / 255.0) * 12);
}

Terrain::Terrain()
{
	size = 0;
}

Terrain::~Terrain()
{

}

void Terrain::draw()
{
	if (size == 0)
		return;

	glPushMatrix();
	glTranslatef(0.5, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glColorPointer(3, GL_FLOAT, sizeof(Vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(3, GL_FLOAT, sizeof(Vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, size);

	glPopMatrix();
}
