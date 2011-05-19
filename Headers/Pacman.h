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
	Ghost(int color, Object * object);

	int color;
	bool scared;
	bool visible;
	Object * obj;
};

class Food
{
	public:
	Food(Mesh mesh, int points);
	int points;

	bool visible;
	Object * obj;
};

class Pacman
{
	public:
		static std::vector<Ghost *> ghosts;
		static std::vector<Food *> food;
		static Object * player;
		static int score, lives;

		static void initialise();
		static void loadGhosts(std::string meshFile, std::string textureFile, int count);
		static void loadFood(std::string meshFile, int count);
		static void loadPlayer(std::string meshFile, std::string textureFile);

		static void drawHUD();
};

std::vector<Food*> Pacman::food;
std::vector<Ghost*> Pacman::ghosts;
Object * Pacman::player;
