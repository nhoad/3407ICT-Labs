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

#define STOPPED 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

class Ghost : public GameEntity
{
	public:
		Ghost(Mesh * mesh, Mat4 * matrix, Vec3 * start, unsigned int vbo, unsigned int texture);
		~Ghost();

		bool scared;
		bool visible;
};

class Food : public GameEntity
{
	public:
		Food(int points, Mesh * mesh, Mat4 * matrix, Vec3 * start, unsigned int vbo, unsigned int texture);
		~Food();

		int points;
		bool visible;
};

class Pacman : public GameEntity
{
	private:
		int direction;

	public:
		Pacman();
		~Pacman();
		void move(int direction, double elapsedTime);
		bool collidesWith(GameEntity * g);
};

class PacmanGame : public Game
{
	private:
		bool keys[350];
		Pacman * pacman;
		Terrain * terrain;
		std::vector<Food*> food_entities;
		std::vector<Ghost*> ghost_entities;
		int lives, score;
		unsigned int pacmanShader;

		void loadWalls();
		void loadGhosts();
		void loadFood();
		void loadPlayer();
		void loadMap();
		void addFood(Food * food);
		void addGhost(Ghost * ghost);

	public:
		~PacmanGame();
		void initialise();
		void draw();
		void update(double elapsedTime);
};
