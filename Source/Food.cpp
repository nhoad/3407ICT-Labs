/**
 * Food object
 * Author: Nathan Hoad.
 * Date: 20th May 2011
 *
 * Description: Implementation of a Food object from Pacman
 */
#include "Pacman.h"

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
