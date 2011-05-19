#pragma once
#include <string>
#include <vector>

#include "Primitives.h"

#define GHOST_PINK 1
#define GHOST_GREEN 2
#define GHOST_BLUE 3
#define GHOST_RED 4
class Ghost
{
	public:
	Ghost(int color, GameEntity * object);

	int color;
	bool scared;
	bool visible;
	GameEntity * obj;
	Vec3 coordinates;

	void draw();
};

class Food
{
	public:
	Food(Mesh mesh, int points);
	int points;

	bool visible;
	GameEntity * obj;
	Vec3 coordinates, color;

	void draw();
};

class Pacman
{
	public:
		Pacman();
		int score;
		int lives;
		GameEntity * obj;
		Vec3 coordinates;

		static std::vector<Ghost *> loadGhosts(std::string meshFile, std::string textureFile, int count);
		static std::vector<Food *> loadFood(std::string meshFile, int count);
		static GameEntity * loadPlayer(std::string meshFile, std::string textureFile);

		static void drawHUD();
		void move(int direction);
		void draw();
};

