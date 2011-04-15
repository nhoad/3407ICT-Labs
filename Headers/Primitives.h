#include <vector>
#include <iostream>
#include "SDL.h"

#pragma once
class Vertex;

typedef std::vector<Vertex> Face;
typedef std::vector<Face> Mesh;

int compareOnY(Vertex a, Vertex b);

class Vertex
{

	float data[8];

	float x, y, z, w;
	float r, g, b, a;

	public:
	Vertex() { }

	Vertex(float x, float y, float z, float w, float r, float g, float b, float a)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;

		data[4] = x;
		data[5] = y;
		data[6] = z;
		data[7] = a;
	}

	float& operator()(int x);
	float operator()(int x) const;

};

class Cube
{
	private:
		/**
			Returns the centre point for an axis.

			For centre point of x axis, use 0.
			For centre point of y axis, use 1.
			For centre point of z axis, use 2.

			\param k the axis to get the centre point for.
			\return centre point of axis k.
		*/
		float centre(int k);

	public:
		Mesh faces;

		float x, y, scale;
		float speed;

		Cube();


		/**
			return centre point of cube on the x axis.

			\return x axis centroid.
		*/
		float centreX();

		/**
			return centre point of cube on the y axis.

			\return y axis centroid.
		*/
		float centreY();

		/**
			return centre point of cube on the z axis.

			\return z axis centroid.
		*/
		float centreZ();

};
