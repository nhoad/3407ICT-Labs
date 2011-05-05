/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#include <vector>
using std::vector;

#include "Primitives.h"

#include <iostream>
using std::ostream;

Object::Object()
{
}

float Object::centre(int k)
{
	float minK, maxK;

	minK = mesh[0][0](k);
	maxK = mesh[0][0](k);

	for (unsigned i=0; i < mesh.size(); i++)
	{
		for (unsigned j=0; j < mesh[i].size(); j++)
		{
			if (mesh[i][j](k) < minK)
				minK = mesh[i][j](k);

			if (mesh[i][j](k) > maxK)
				maxK = mesh[i][j](k);
		}
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
	return (a(1) < b(1));
}

float Vertex::operator()(int x) const
{
	return pos[x];
}

float& Vertex::operator()(int x)
{
	return pos[x];
}

ostream & operator<<(ostream & o, const Vertex & v)
{
	for (int i=0; i < 8; i++)
	{
		o.width(10);
		o << v(i);
	}

	return o;
}
