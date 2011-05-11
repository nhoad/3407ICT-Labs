/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#include <vector>
#include <iostream>
using std::cout;
using std::endl;
#include "SDL.h"
#include "glut.h"
#include "Transformation.h"

#pragma once
class Vertex;

typedef std::vector<Vertex> Mesh;

/**
	Compare two vertices on their Y values. Used for sorting points in a triangle for edge list generation.

	\param a first Vertex.
	\param b second Vertex to compare to a.
	\return -1, 0 or +1 depending on the value of a and b's y values.
*/
int compareOnY(Vertex a, Vertex b);

/**
	Vertex class for representing a point in an object
*/
struct Vertex
{
	union {
		struct {
			float x, y, z, w;
		};
		float pos[4];
	};

	union {
		struct {
			float nx, ny, nz;
		};
		float normal[3];
	};

	union {
		struct {
			float tx, ty;
		};
		float tex[2];
	};

	Vertex(float x=0.0, float y=0.0, float z=0.0, float w=1.0,
			 float nx=0.0, float ny=0.0, float nz=1.0,
			 float tx=0.0, float ty=0.0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;

		this->nx = nx;
		this->ny = ny;
		this->nz = nz;
		this->tx = tx;
		this->ty = ty;
	};

	friend std::ostream & operator<<(std::ostream & o, const Vertex & v);
	float operator()(int x) const;
	float& operator()(int x);
};

/**
	Object class.
*/
class Object
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
		Mesh mesh;
		Mat4 matrix;

		GLuint vbo;
		unsigned int shaderProgram;

		Object();


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
