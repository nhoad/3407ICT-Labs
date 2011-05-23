#include "Pacman.h"
#include <iostream>
using std::cout;
using std::endl;

Pacman::Pacman() : GameEntity("Assets/sphere.obj", "Assets/Checkerboard.png", new Mat4(), new Vec3(12, 0, 9))
{
	Mat4 m = Mat4::scale(50, 50, 50);
	transform(&m);
	this->direction = STOPPED;
}

Pacman::~Pacman()
{

}

void Pacman::move(int direction, double elapsedTime)
{
	double speed = 3 * elapsedTime;
	switch (direction)
	{
		case UP:
			GameEntity::move(-speed, 0, 0);
			break;
		case DOWN:
			GameEntity::move(speed, 0, 0);
			break;
		case LEFT:
			GameEntity::move(0, 0, speed);
			break;
		case RIGHT:
			GameEntity::move(0, 0, -speed);
			break;
		default:
			break;
	}

}

bool Pacman::collidesWith(GameEntity * g)
{
	Vec3 v = getCoordinates();
	Vec3 o_v = g->getCoordinates();

	float x, z, o_x, o_z;

	x = v.x;
	z = v.z;

	o_x = o_v.x;
	o_z = o_v.z;

	if (x > o_x - 14 && x < o_x + 14)
	{
		if (z > o_z - 14 && z < o_z + 14)
			return true;
	}

	return false;
}
