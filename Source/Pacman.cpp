#include "Pacman.h"

Pacman::Pacman(int lives) : GameEntity("Assets/sphere.obj", "Assets/Checkerboard.png", new Mat4(), new Vec3())
{
	Mat4 m = Mat4::scale(20, 20, 20);
	transform(&m);
	this->lives = lives;
	this->score = 0;
	this->direction = STOPPED;
}

Pacman::~Pacman()
{

}

void Pacman::move(int direction, double elapsedTime)
{
	double speed = 1 * elapsedTime;
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
