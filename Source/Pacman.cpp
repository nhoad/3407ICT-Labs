#include "Pacman.h"
#include <iostream>
using namespace std;

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

void Pacman::move(int direction)
{
	cout << "moving" << endl;
	cout << direction << endl;
	switch (direction)
	{
		case UP:
			GameEntity::move(-1, 0, 0);
			break;
		case DOWN:
			GameEntity::move(1, 0, 0);
			break;
		case LEFT:
			GameEntity::move(0, 0, 1);
			break;
		case RIGHT:
			GameEntity::move(0, 0, -1);
			break;
		default:
			cout << "ntohing" << endl;
			break;
	}

}
