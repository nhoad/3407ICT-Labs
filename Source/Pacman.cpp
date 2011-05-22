#include "Pacman.h"

Pacman::Pacman(int lives) : GameEntity("Assets/Cube.obj", "Assets/Checkerboard.png", new Mat4(), new Vec3())
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

}
