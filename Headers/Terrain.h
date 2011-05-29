#pragma once
#include <vector>
#include "Vec3.h"
#include "Game.h"

class Terrain
{
	private:
		int vbo, color_vbo;
		std::vector<Vec3> mesh;

		bool testMovement(BoundingBox * b);

	public:
		Terrain(unsigned int vbo, unsigned int color_vbo, std::vector<Vec3> mesh);
		~Terrain();

		static float getHeight(std::vector<float> * heights, int x, int y, int y_step);
		static Vec3 getColour(std::vector<float> * heights, int x, int y, int y_step);

		void draw();
		int size();

		bool canGoUp(GameEntity * g);
		bool canGoDown(GameEntity * g);
		bool canGoLeft(GameEntity * g);
		bool canGoRight(GameEntity * g);

};
