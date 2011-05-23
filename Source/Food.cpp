#include "Pacman.h"
#include <iostream>
using std::cout;
using std::endl;

Food::Food(int points, Mesh * mesh, Mat4 * matrix, Vec3 * start, unsigned int vbo, unsigned int texture) :
	GameEntity(mesh, matrix, texture, start)
{
	setVBO(vbo);
	this->points = points;
	this->visible = true;
}

Food::~Food()
{

}
