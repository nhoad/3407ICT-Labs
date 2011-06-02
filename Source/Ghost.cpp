/**
 * Ghost object implementation
 * Author: Nathan Hoad.
 * Date: 20th May 2011
 *
 * Description: Implementation of a Ghost object
 */
#include "Pacman.h"
#include "Terrain.h"

Ghost::Ghost(Mesh * mesh, Mat4 * matrix, Vec3 * start, unsigned int vbo, unsigned int texture) :
	GameEntity(mesh, matrix, texture, start)
{
	setVBO(vbo);
	scared = false;
	visible = true;
}

Ghost::~Ghost() { }
