/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */

#pragma once
#include <vector>
#include <iostream>

#include "Mat4.h"
#include "Vec3.h"

#include "SDL.h"
#include "glew.h"

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

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
class Vertex
{
	public:
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

	Vertex(float x=0.0, float y=0.0, float z=0.0,
			 float nx=0.0, float ny=0.0, float nz=1.0,
			 float tx=0.0, float ty=0.0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 1;

		this->nx = nx;
		this->ny = ny;
		this->nz = nz;
		this->tx = tx;
		this->ty = ty;
	};

	friend std::ostream & operator<<(std::ostream & o, const Vertex & v);
};

/**
	GameEntity class.
*/
class GameEntity
{
	private:
		/**
			Returns the centre point for an axis.

			\param k the axis to get the centre point of, using either X_AXIS, Y_AXIS or Z_AXIS.
			\return centre point of axis k.
		*/
		float centre(int k);

	public:
		Mesh mesh;
		Mat4 matrix;
		unsigned int shader, vbo, texture;

		GameEntity(std::string meshFile= "", std::string textureFile = "", Mat4 m = Mat4());
		GameEntity(Mesh mesh, Mat4 m = Mat4());

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

		/**
		 * Make openGL calls to draw this object
		 */
		void draw();

		/**
		 * Send the mesh to the VBO and set vbo variable accordingly
		 */
		void buffer();
};

class Terrain
{
	public:
		Terrain();
		~Terrain();

		int vbo, color_vbo, size;

		static float getHeight(std::vector<float> * heights, int x, int y, int y_step);
		static Vec3 getColour(std::vector<float> * heights, int x, int y, int y_step);

		void draw();
};
