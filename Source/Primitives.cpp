/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 */
#include <vector>
using std::vector;

#include "Primitives.h"

#include <iostream>
using std::ostream;

Object::Object()
{
	x = 0;
	y = 0;
	scale = 1;
}

float Object::centre(int k)
{
	float minK, maxK;

	minK = faces[0][0](k);
	maxK = faces[0][0](k);

	for (unsigned i=0; i < faces.size(); i++)
	{
		for (unsigned j=0; j < faces[i].size(); j++)
		{
			if (faces[i][j](k) < minK)
				minK = faces[i][j](k);

			if (faces[i][j](k) > maxK)
				maxK = faces[i][j](k);
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
	return data[x];
}

float& Vertex::operator()(int x)
{
	return data[x];
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
