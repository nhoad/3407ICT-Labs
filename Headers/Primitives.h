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
#include "Vec2.h"

#include "SDL.h"
#include "glew.h"

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define NONE_AXIS 4

class GameEntity;
class Vertex;

class Mesh
{
	private:
		bool use_normals, use_texture;
		std::vector<Vertex> points;

	public:
		Mesh();
		void push_back(Vertex v);
		int size();
		void clear();
		void useTextureCoords();
		void useNormals();

		bool textures();
		bool normals();

		Vertex operator[](int x) const;
		Vertex& operator[](int x);

		std::vector<Vertex> data();
};

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

	Vertex(Vec3 coords, Vec3 normals, Vec2 tex)
	{
		this->x = coords(0);
		this->y = coords(1);
		this->z = coords(2);
		this->w = 1;

		this->nx = normals(0);
		this->ny = normals(1);
		this->nz = normals(2);

		this->tx = tex(0);
		this->ty = tex(1);
	}

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

