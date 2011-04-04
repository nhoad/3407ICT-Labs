#include <vector>
#include <iostream>


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

	friend std::ostream & operator<<(std::ostream & o, const Vertex v)
	{
		o << "x: " << v.x << std::endl;
		o << "y: " << v.y << std::endl;
		o << "z: " << v.z << std::endl;

		return o;
	}

};

typedef std::vector<Vertex> Mesh;

float mesh_z(const Mesh & mesh);
float mesh_y(const Mesh & mesh);
float mesh_x(const Mesh & mesh);
