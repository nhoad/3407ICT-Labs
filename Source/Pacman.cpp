#include "Pacman.h"

Pacman::Pacman(int lives) : GameEntity("Assets/Cube.obj", "Assets/Checkerboard.png", new Mat4(), new Vec3())
{
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
