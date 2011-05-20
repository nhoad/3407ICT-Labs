/**
	Pacman classes.
	Author: Nathan Hoad
	Date: 20th May 2011

	Description: This is a set of all the classes I use for Pacman. I know that
	some will say it's poor design to put so many classes in a header file, but
	I'm not creating 4 new files for tiny, 8 line classes.
*/

#pragma once

#include "Game.h"

#include <string>
#include <vector>

#define GHOST_PINK 1
#define GHOST_GREEN 2
#define GHOST_BLUE 3
#define GHOST_RED 4

#define STOPPED 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

class Ghost : public GameEntity
{
	public:
		Ghost(int color);
		~Ghost();

		bool scared;
		bool visible;
};

class Food : public GameEntity
{
	public:
		Food(int points);
		~Food();

		int points;
		bool visible;
};

class Pacman : public GameEntity
{
	private:
		int score, lives, direction;

	public:
		Pacman(int lives);
		~Pacman();
		void move(int direction);
};

class PacmanGame : public Game
{
	private:
		bool keys[256];
		Pacman * pacman;
		Terrain * terrain;
		void loadGhosts();
		void loadFood();
		void loadPlayer();
		void loadMap();

	public:
		~PacmanGame();
		void initialise();
		void draw();
		void update();
};
