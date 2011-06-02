#pragma once
#include <vector>
#include "Vec3.h"
#include "Game.h"

/// Terrain class, based on height maps
class Terrain
{
	private:
		int vbo, color_vbo;
		std::vector<Vec3> mesh;

		/**
			See if the bounding box is legally positioned

			\param b bounding box to test
			\return true of the bounding box doesn't collide with terrain, false if it does
		*/
		bool testMovement(BoundingBox * b);

	public:
		Terrain(unsigned int vbo, unsigned int color_vbo, std::vector<Vec3> mesh);
		~Terrain();

		/**
			Get the height value of a map at a specific point

			\param heights the height values
			\param x x coordinate of the requested height value
			\param y y coordinate of the requested height value
			\param y_step the width of the image
			\return height of point x, y according to the heights
		*/
		static float getHeight(std::vector<float> * heights, int x, int y, int y_step);

		/**
			Get the color value of a map at a specific point

			\param heights the height values
			\param x x coordinate of the requested height value
			\param y y coordinate of the requested height value
			\param y_step the width of the image
			\return color of point x, y according to the heights
		*/
		static Vec3 getColour(std::vector<float> * heights, int x, int y, int y_step);

		/// draw the Terrain object
		void draw();

		/// size of the Mesh
		int size();

		/**
			Collision detection for moving up.

			\param g GameEntity to test.
			\return true if the GameEntity can move up, false otherwise
		*/
		bool canGoUp(GameEntity * g);

		/**
			Collision detection for moving down.

			\param g GameEntity to test.
			\return true if the GameEntity can move down, false otherwise
		*/
		bool canGoDown(GameEntity * g);

		/**
			Collision detection for moving left.

			\param g GameEntity to test.
			\return true if the GameEntity can move left, false otherwise
		*/
		bool canGoLeft(GameEntity * g);

		/**
			Collision detection for moving right.

			\param g GameEntity to test.
			\return true if the GameEntity can move right, false otherwise
		*/
		bool canGoRight(GameEntity * g);
};
