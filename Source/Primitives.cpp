/**
 * Primitive implementations
 * Author: Nathan Hoad.
 * Date: 20th May 2011
 *
 * Description: Implementation of various primitives such as a Vertex, and Mesh
 */
#include "glew.h"

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include <iostream>
using std::ostream;

using std::string;

#include "Primitives.h"
#include "Loader.h"
#include "Vec3.h"
#include "Game.h"

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

Mesh::Mesh()
{
	use_normals = false;
	use_texture = false;
}

void Mesh::useTextureCoords()
{
	use_texture = true;
}

void Mesh::useNormals()
{
	use_normals = true;
}

void Mesh::push_back(Vertex v)
{
	points.push_back(v);
}

void Mesh::clear()
{
	points.clear();
}

int Mesh::size()
{
	return points.size();
}

bool Mesh::textures()
{
	return use_texture;
}

bool Mesh::normals()
{
	return use_normals;
}

vector<Vertex> Mesh::data()
{
	return points;
}

Vertex& Mesh::operator[](int x)
{
	return points[x];
}

Vertex Mesh::operator[](int x) const
{
	return points[x];
}

