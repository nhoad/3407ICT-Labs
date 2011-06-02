/**
 * Pacman class implementation
 * Author: Nathan Hoad.
 * Date: 20th May 2011
 *
 * Description: Implementation of the Pacman class for a Pacman player object
 */
#include "Pacman.h"

Pacman::Pacman() : GameEntity("Assets/sphere.obj", "Assets/pacmanTexture.png", new Mat4(), new Vec3(12, 0, 9))
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
/*	Vec3 v = getWorldCoordinates();
	Vec3 o_v = g->getWorldCoordinates();

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

	return false;*/

	Vec3 o_v = g->getWorldCoordinates();
	return this->getBoundingBox()->collisionAt(&o_v);
}
