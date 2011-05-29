#include "Game.h"

using std::vector;

#include <iostream>
using std::cout;
using std::endl;


BoundingBox::BoundingBox(Mat4 * matrix, Vec3 * coordinates, Mesh * mesh)
{
	this->matrix = matrix;
	this->coordinates = coordinates;

	vector<Vertex> data = mesh->data();

	min.x = data[0].x;
	min.y = data[0].y;
	min.z = data[0].z;

	max.x = data[0].x;
	max.y = data[0].y;
	max.z = data[0].z;

	for (int i=1; i < data.size(); i++)
	{
		for (int j=0; j < 3; j++)
		{
			if (min(j) > data[i].pos[j])
				min(j) = data[i].pos[j];

			if (max(j) < data[i].pos[j])
				max(j) = data[i].pos[j];
		}
	}

	calculateWorld(NONE_AXIS, 0);
}

void BoundingBox::calculateWorld(int offset_axis, float amount)
{
	worldMin = min;
	worldMax = max;

	worldMax.x += coordinates->x;
	worldMax.y += coordinates->y;
	worldMax.z += coordinates->z;

	worldMin.x += coordinates->x;
	worldMin.y += coordinates->y;
	worldMin.z += coordinates->z;

	worldMin = Mat4::mul(*matrix, worldMin);
	worldMax = Mat4::mul(*matrix, worldMax);

	if (offset_axis != NONE_AXIS)
	{
		worldMin(offset_axis) += amount;
		worldMax(offset_axis) += amount;
	}
}

bool BoundingBox::collisionAt(Vec3 * position)
{
	// in the real world I'd use the y axis as well, but this game only works in the
	// 2d perspective for collisions, so considering the y axis isn't a good idea
	if (worldMin.x < position->x && worldMin.z < position->z)
		if (worldMax.x > position->x && worldMax.z > position->z)
			return true;

	return false;
}
