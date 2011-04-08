#include <vector>
#include <iostream>

#pragma once
class Vertex;

typedef std::vector<Vertex> Mesh;

struct Vertex
{

	float x, y, z, w;
	float r, g, b, a;

	public:
	Vertex() { }

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

	friend std::ostream & operator<<(std::ostream & o, const Vertex v)
	{
		o << "x: " << v.x << std::endl;
		o << "y: " << v.y << std::endl;
		o << "z: " << v.z << std::endl;

		return o;
	}

};

float centreX(const Mesh & mesh);
float centreY(const Mesh & mesh);
float centreZ(const Mesh & mesh);

