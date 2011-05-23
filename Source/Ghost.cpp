#include "Pacman.h"

Ghost::Ghost(Mesh * mesh, Mat4 * matrix, Vec3 * start, unsigned int vbo, unsigned int texture) :
	GameEntity(mesh, matrix, texture, start)
{
	setVBO(vbo);
	scared = false;
	visible = true;
}


Ghost::~Ghost() { }
