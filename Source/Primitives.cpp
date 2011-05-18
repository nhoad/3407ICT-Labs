/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#include <vector>
using std::vector;

#include "Primitives.h"
#include "Loader.h"

#include <iostream>
using std::ostream;

using std::string;

Object::Object(string meshFile, string textureFile, Mat4 m)
{
	if (meshFile.size() > 0)
		this->mesh = Loader::readMesh(meshFile);

	if (textureFile.size() > 0)
		this->texture = Loader::loadTexture(textureFile);
	else
		this->texture = 0;

	this->matrix = m;
}

Object::Object(Mesh mesh, Mat4 m)
{
	this->mesh = mesh;
	this->texture = 0;
	this->matrix = m;

}

float Object::centre(int k)
{
	float minK, maxK;

	minK = mesh[0].pos[k];
	maxK = mesh[0].pos[k];

	for (unsigned i=0; i < mesh.size(); i++)
	{
		if (mesh[i].pos[k] < minK)
			minK = mesh[i].pos[k];

		if (mesh[i].pos[k] > maxK)
			maxK = mesh[i].pos[k];
	}

	return (minK + maxK) / 2.0f;
}

float Object::centreY()
{
	return centre(1);
}

float Object::centreZ()
{
	return centre(2);
}

float Object::centreX()
{
	return centre(0);
}

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
