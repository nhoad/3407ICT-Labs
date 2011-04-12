/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 */
#include <vector>
using std::vector;

#include "Primitives.h"

float centreZ(const Mesh & mesh)
{
	float minZ, maxZ;

	minZ = mesh[0].z;
	maxZ = mesh[0].z;

	for (unsigned i=1; i < mesh.size(); i++)
	{
		if (mesh[i].z < minZ)
			minZ = mesh[i].z;

		if (mesh[i].z > maxZ)
			maxZ = mesh[i].z;
	}
	return (minZ + maxZ) / 2.0f;
}

float centreY(const Mesh & mesh)
{
	float minY, maxY;

	minY = mesh[0].y;
	maxY = mesh[0].y;

	for (unsigned i=1; i < mesh.size(); i++)
	{
		if (mesh[i].y < minY)
			minY = mesh[i].y;

		if (mesh[i].y > maxY)
			maxY = mesh[i].y;
	}
	return (minY + maxY) / 2.0f;
}

float centreX(const Mesh & mesh)
{
	float minX, maxX;

	minX = mesh[0].x;
	maxX = mesh[0].x;

	for (unsigned i=1; i < mesh.size(); i++)
	{
		if (mesh[i].x < minX)
			minX = mesh[i].x;

		if (mesh[i].x > maxX)
			maxX = mesh[i].x;
	}
	return (minX + maxX) / 2.0f;
}

int compareOnY(Point a, Point b)
{
	return (a.y < b.y);
}
