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

class Ghost : public GameEntity
{
	public:
		Ghost(int color);
		virtual ~Ghost();

		bool scared;
		bool visible;
};

class Food : public GameEntity
{
	public:
		Food(Mesh mesh, int points);
		virtual ~Food();

		int points;
		bool visible;
};

class Pacman : public GameEntity
{
	private:
		int score, lives, direction;

	public:
		Pacman();
		virtual ~Pacman();

		void move(int direction);
		void draw(float timeFrame);
};

class PacmanGame : public Game
{
	public:
		void loadGhosts(std::string meshFile, std::string textureFile, int count);
		void loadFood(std::string meshFile, int count);
		Pacman * loadPlayer(std::string meshFile, std::string textureFile);
};
