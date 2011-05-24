/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#include "glew.h"

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include <iostream>
using std::ostream;

using std::string;

#include "Primitives.h"
#include "Loader.h"
#include "Vec3.h"

int compareOnY(Vertex a, Vertex b)
{
	return (a.y < b.y);
}

ostream & operator<<(ostream & o, const Vertex & v)
{
	for (int i=0; i < 4; i++)
	{
		o.width(10);
		o << v.pos[i];
	}

	for (int i=0; i < 3; i++)
	{
		o.width(10);
		o << v.normal[i];
	}

	for (int i=0; i < 2; i++)
	{
		o.width(10);
		o << v.tex[i];
	}

	return o;
}

Mesh::Mesh()
{
	use_normals = false;
	use_texture = false;
}

void Mesh::useTextureCoords()
{
	use_texture = true;
}

void Mesh::useNormals()
{
	use_normals = true;
}

void Mesh::push_back(Vertex v)
{
	points.push_back(v);
}

void Mesh::clear()
{
	points.clear();
}

int Mesh::size()
{
	return points.size();
}

bool Mesh::textures()
{
	return use_texture;
}

bool Mesh::normals()
{
	return use_normals;
}

vector<Vertex> Mesh::data()
{
	return points;
}

Vertex& Mesh::operator[](int x)
{
	return points[x];
}

Vertex Mesh::operator[](int x) const
{
	return points[x];
}

Terrain::Terrain(unsigned int vbo, unsigned int color_vbo, vector<Vec3> mesh)
{
	this->vbo = vbo;
	this->color_vbo = color_vbo;
	this->mesh = mesh;
}

float Terrain::getHeight(std::vector<float> * heights, int x, int y, int y_step)
{
	return (*heights)[x + (y * y_step)];
}

Vec3 Terrain::getColour(std::vector<float> * heights, int x, int y, int y_step)
{
	float height = Terrain::getHeight(heights, x, y, y_step);

	if (height == 0)
		return Vec3();

	return Vec3(0.13, 0.24, (height / 255.0) * 12);
}

Terrain::~Terrain()
{

}

void Terrain::draw()
{
	if (mesh.size() == 0)
		return;

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glPushMatrix();

	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glColorPointer(3, GL_FLOAT, sizeof(Vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(3, GL_FLOAT, sizeof(Vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// disable mesh mode
	glDrawArrays(GL_TRIANGLES, 0, mesh.size());

	glPopMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

#define step_size 5

bool Terrain::canGoRight(Vec3 * v, Mat4 * m)
{
	Vec4 vec = *v;
	vec.z--;

	vec = Mat4::mul(*m, vec);

	for (int i=0; i < mesh.size(); i++)
		if (mesh[i].y > 0 && vec.x > mesh[i].x-step_size && vec.x < mesh[i].x+step_size &&
			 vec.z > mesh[i].z-step_size && vec.z < mesh[i].z)
			return false;

	return true;
}

bool Terrain::canGoLeft(Vec3 * v, Mat4 * m)
{
	Vec4 vec = *v;
	vec.z++;

	vec = Mat4::mul(*m, vec);

	for (int i=0; i < mesh.size(); i++)
		if (mesh[i].y > 0 && vec.x > mesh[i].x-step_size && vec.x < mesh[i].x+step_size &&
			 vec.z > mesh[i].z && vec.z < mesh[i].z+step_size)
			return false;

	return true;
}

bool Terrain::canGoDown(Vec3 * v, Mat4 * m)
{
	Vec4 vec = *v;
	vec.x++;

	vec = Mat4::mul(*m, vec);

	for (int i=0; i < mesh.size(); i++)
		if (mesh[i].y > 0 && vec.x > mesh[i].x && vec.x < mesh[i].x+step_size &&
			 vec.z > mesh[i].z-step_size && vec.z < mesh[i].z+step_size)
			return false;

	return true;
}
bool Terrain::canGoUp(Vec3 * v, Mat4 * m)
{
	Vec4 vec = *v;
	vec.x--;

	vec = Mat4::mul(*m, vec);

	for (int i=0; i < mesh.size(); i++)
		if (mesh[i].y > 0 && vec.x > mesh[i].x-step_size && vec.x < mesh[i].x &&
			 vec.z > mesh[i].z-step_size && vec.z < mesh[i].z+step_size)
			return false;

	return true;
}

int Terrain::size()
{
	return mesh.size();
}
