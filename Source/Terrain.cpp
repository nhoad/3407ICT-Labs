/**
 * Terrain implementation
 * Author: Nathan Hoad.
 * Date: 20th May 2011
 *
 * Implementation of the Terrain class for loading terrain from a heightmap
 */
#include "Terrain.h"
#include "glew.h"

using std::vector;

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

bool Terrain::canGoRight(GameEntity * g)
{
	BoundingBox * b = g->getBoundingBox();

	b->calculateWorld(Z_AXIS, -2);
	bool result = testMovement(b);
	b->calculateWorld(Z_AXIS, 2);

	return result;
}

bool Terrain::canGoLeft(GameEntity * g)
{
	BoundingBox * b = g->getBoundingBox();

	b->calculateWorld(Z_AXIS, 2);
	bool result = testMovement(b);
	b->calculateWorld(Z_AXIS, -2);

	return result;
}

bool Terrain::canGoUp(GameEntity * g)
{
	BoundingBox * b = g->getBoundingBox();

	b->calculateWorld(X_AXIS, -2);
	bool result = testMovement(b);
	b->calculateWorld(X_AXIS, 2);

	return result;
}

bool Terrain::canGoDown(GameEntity * g)
{
	BoundingBox * b = g->getBoundingBox();

	b->calculateWorld(X_AXIS, 2);
	bool result = testMovement(b);
	b->calculateWorld(X_AXIS, -2);

	return result;
}

bool Terrain::testMovement(BoundingBox * b)
{
	bool result = true;

	for (int i=0; i < mesh.size(); i++)
		if (mesh[i].y > 0 && b->collisionAt(&mesh[i]))
		{
			result = false;
			break;
		}

	return result;

}

int Terrain::size()
{
	return mesh.size();
}
