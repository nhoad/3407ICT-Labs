#include <vector>
#include <iostream>
#include "SDL.h"

#pragma once
class Vertex;

typedef std::vector<Vertex> Mesh;

class Point
{
	public:
	int x, y, z;
	Uint8 r, g, b;

	Point(int x, int y, Uint8 r=0, Uint8 g=0, Uint8 b=0) : x(x), y(y), z(z), r(r), g(g), b(b) {}

};

int compareOnY(Point a, Point b);

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

struct Cube
{
	public:
		Mesh mesh;
		float x, y, scale;
		float speed;

		Cube();

		float centreX();
		float centreY();
		float centreZ();

		std::vector<Point> getPoints(const int start, const int end);
};
