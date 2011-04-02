#include <vector>

#pragma once
struct Vertex
{

	float x, y, z, w;
	float r, g, b, a;

	Vertex(float x, float y, float z, float w, float r, float g, float b, float a)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;

		this->r = x;
		this->g = y;
		this->b = z;
		this->a = a;
	}

};

typedef std::vector<Vertex> Mesh;
