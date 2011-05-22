#include "Pacman.h"

Ghost::Ghost(int color)
{
	switch (color)
	{
		case GHOST_PINK:
		case GHOST_GREEN:
		case GHOST_BLUE:
		case GHOST_RED:
			GameEntity::GameEntity("Assets/Ghost.obj", "Assets/GhostRed.png", new Mat4());
			break;
	}
	this->color = color;
	scared = false;
	visible = false;
	obj = object;
}


Ghost::~Ghost()
{
	GameEntity::~GameEntity();
}
