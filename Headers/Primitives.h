/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#include <vector>
#include <iostream>
#include "SDL.h"
#include "Transformation.h"

#pragma once
class Vertex;

typedef std::vector<Vec4> Face;
typedef std::vector<Face> Mesh;

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
class Vertex
{
	float data[8];

	public:

	/**
		Default constructor. Sets everything to zero.
	*/
	Vertex()
	{
		for (int i=0; i < 8; i++)
			data[i] = 0;
	}

	/**
		Constructor.

		\param x the x coordinate
		\param y the y coordinate
		\param z the z coordinate
		\param w the w coordinate
		\param r red colour value
		\param g green colour value
		\param b blue colour value
		\param a alpha value
		\return description
	*/
	Vertex(float x, float y, float z, float w, float r, float g, float b, float a)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;

		data[4] = r;
		data[5] = g;
		data[6] = b;
		data[7] = a;
	}

	/**
		Overloaded operator for accessing data

		\param x the index of data.
		\return float at x in the vertex.
	*/
	float& operator()(int x);

	/**
		Overloaded operator for accessing data

		\param x the index of data.
		\return float at x in the vertex.
	*/
	float operator()(int x) const;

	/**
		Displays nice output!
	*/
	friend std::ostream & operator<<(std::ostream & o, const Vertex & v);

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
		Mesh faces;

		float x, y;

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
