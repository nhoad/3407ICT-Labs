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

#include "Primitives.h"
#include "Loader.h"
#include "Vec3.h"

#include <iostream>
using std::ostream;

using std::string;

GameEntity::GameEntity(string meshFile, string textureFile, Mat4 m)
{
	if (meshFile.size() > 0)
		this->mesh = Loader::readMesh(meshFile);

	if (textureFile.size() > 0)
		this->texture = Loader::loadTexture(textureFile);
	else
		this->texture = 0;

	this->matrix = m;
}

GameEntity::GameEntity(Mesh mesh, Mat4 m)
{
	this->mesh = mesh;
	this->texture = 0;
	this->matrix = m;

}

float GameEntity::centre(int k)
{
	float minK, maxK;

	minK = mesh[0].pos[k];
	maxK = mesh[0].pos[k];

	for (unsigned i=0; i < mesh.size(); i++)
	{
		if (mesh[i].pos[k] < minK)
			minK = mesh[i].pos[k];

		if (mesh[i].pos[k] > maxK)
			maxK = mesh[i].pos[k];
	}

	return (minK + maxK) / 2.0f;
}

float GameEntity::centreY()
{
	return centre(1);
}

float GameEntity::centreZ()
{
	return centre(2);
}

float GameEntity::centreX()
{
	return centre(0);
}

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

void GameEntity::draw()
{
	glPushMatrix();

	glMultMatrixf(matrix);

	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mesh[0].tx);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), &mesh[0].nx);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_QUADS, 0, mesh.size());

	glPopMatrix();

}

void GameEntity::buffer()
{
	if (mesh.size() == 0)
		return;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.size(), &mesh[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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

	return Vec3(0.13, 0.24, (height / 255.0)+ 0.2);
}

Terrain::Terrain()
{
	size = 0;
}

Terrain::~Terrain()
{

}

void Terrain::draw()
{
	if (size == 0)
		return;

	glPushMatrix();
	glTranslatef(0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glColorPointer(3, GL_FLOAT, sizeof(Vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(3, GL_FLOAT, sizeof(Vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, size);

	glPopMatrix();
}
