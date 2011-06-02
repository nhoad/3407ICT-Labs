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
#include "Terrain.h"

#include <string>
#include <vector>

#define STOPPED 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

class Pacman;

/// Ghost class. Contains states for being scared and invisible, although they're unused so far.
class Ghost : public GameEntity
{
	public:
		/**
			Constructor.

			\param mesh mesh of the Ghost.
			\param matrix matrix of the Ghost
			\param start the starting coordinates
			\param vbo id the vbo of the mesh
			\param texture id of the ghost's texture
		*/
		Ghost(Mesh * mesh, Mat4 * matrix, Vec3 * start, unsigned int vbo, unsigned int texture);

		/// Destructor
		~Ghost();

		bool scared;
		bool visible;
};

/// Food class. Represents energy in the pacman game, for getting points
class Food : public GameEntity
{
	public:
		/**
			Constructor.

			\param points how many points this Food object is worth once collected
			\param mesh the mesh of the Food object
			\param matrix the transformation matrix of the object
			\param start the starting coordinates of the object
			\param vbo id of the buffered mesh
			\param texture id of the buffered texture
		*/
		Food(int points, Mesh * mesh, Mat4 * matrix, Vec3 * start, unsigned int vbo, unsigned int texture);

		/// Destructor
		~Food();

		int points;
		bool visible;
};

/// Pacman class. Represents a player
class Pacman : public GameEntity
{
	private:
		int direction;

	public:
		/// Constructor
		Pacman();

		/// Destructor
		~Pacman();

		/**
			Move pacman

			\param direction the direction to move pacman. Should be one of UP, LEFT, DOWN or RIGHT
			\param elapsedTime the amount of elapsedTime in seconds since the last frame. CRUCIAL for timebased movement
		*/
		void move(int direction, double elapsedTime);

		/**
			Collision detection. Checks against Food or Ghost, or anything extending GameEntity

			\param g the GameEntity to test.
			\return true if a collision occured, false otherwise
		*/
		bool collidesWith(GameEntity * g);
};

/// PacmanGame class for representing the pacman game.
class PacmanGame : public Game
{
	private:
		bool keys[350];
		Pacman * pacman;
		Terrain * terrain;
		std::vector<Food*> food_entities;
		std::vector<Ghost*> ghost_entities;
		unsigned int pacmanShader;

		/// load the ghosts. Buffers them, loads textures, mesh. The works!
		void loadGhosts();

		/// load the food objects. Buffers everything necessary, loads textures and mesh
		void loadFood();

		/// Load the player.
		void loadPlayer();

		/// Load the terrain
		void loadMap();

		/// add a food object to the list of food objects
		void addFood(Food * food);

		/// add a ghost object to the list of ghost objects
		void addGhost(Ghost * ghost);

	public:
		int lives, score;
		~PacmanGame();

		/// Initialise the game. This loads all the necessary objects required for the game. Should only be called once
		void initialise();

		/// Draws the game entities, terrain. Should be called every render loop
		void draw();

		/// Update the dynamic parts of the game (e.g. Ghosts movement, Pacman) based on the elapsedTime since the previous frame
		void update(double elapsedTime);
};
