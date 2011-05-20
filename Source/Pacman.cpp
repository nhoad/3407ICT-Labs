#include "glew.h"

#define LEFT 2
#define RIGHT 3
#define UP 4
#define DOWN 5

#include "Pacman.h"
#include "Loader.h"

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <cmath>

#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>

Food::Food(Mesh mesh, int points)
{
	this->points = points;
	this->obj = new GameEntity(mesh);
}

Ghost::Ghost(int color, GameEntity * object)
{
	this->color = color;
	scared = false;
	visible = false;
	obj = object;
}

vector<Ghost*> Pacman::loadGhosts(string meshFile, string textureFile, int count)
{
	Mesh mesh = Loader::readMesh(meshFile);
	vector<Ghost*> ghosts;

	for (int i=0; i < 4; i++)
		ghosts.push_back(new Ghost(i, new GameEntity(mesh)));

	return ghosts;
}

vector<Food*> Pacman::loadFood(std::string meshFile, int count)
{
	Mesh m = Loader::readMesh(meshFile);
	vector<Food*> food;

	unsigned buffer;

	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m.size(),
		&m[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	int x = 3;

	for (int i=0; i < count; i++)
	{
		Food * f = new Food(m, 10);

		// this needs to be modified to put them in the correct place
		f->coordinates = Vec3(x, 0, 3 + (i * 2));
		f->obj->matrix = Mat4::scale(20, 20, 20);
		f->obj->vbo = buffer;
		f->color = Vec3(0, 1, 1);

		if (i % 10 == 0)
		{
			f->points = 20;
			f->color = Vec3(0, 1, 0);
			f->obj->matrix = Mat4::scale(20, 20, 20);
		}


		food.push_back(f);
	}

	return food;
}

GameEntity* Pacman::loadPlayer(std::string meshFile, std::string textureFile)
{
	GameEntity * player = new GameEntity(meshFile, textureFile);

	glGenBuffers(1, &player->vbo);

	glBindBuffer(GL_ARRAY_BUFFER, player->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * player->mesh.size(),
		&player->mesh[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return player;
}

Pacman::Pacman()
{
	obj = Pacman::loadPlayer("Assets/Cube_CubeMapped.obj", "");
	coordinates = Vec3(15, 4, 12);
	direction = 6;

	obj->matrix = Mat4::scale(4, 4, 4);

	//Pacman::loadGhosts("Assets/Ghost.obj", "Assets/Ghost.png", 4);
}

void Pacman::move(int direction)
{
	this->direction = direction;
}

void Ghost::draw()
{
	Mat4 m = obj->matrix;
	obj->matrix = Mat4::translate(coordinates) * m;
	obj->draw();
	obj->matrix = m;
}

void Food::draw()
{
	Mat4 m = obj->matrix;
	obj->matrix = Mat4::translate(coordinates) * m;
	glColor3f(color(0), color(1), color(2));
	obj->draw();
	obj->matrix = m;
}

void Pacman::draw(float timeFrame)
{
	switch (direction)
	{
		case UP:
		{
			coordinates(0) -= 1 * timeFrame;

			int a = coordinates(0) * 10;
			int b = round(coordinates(0)) * 10;

			if (a == b)
				direction = 6;
			break;
		}
		case DOWN:
		{
			coordinates(0) += 1 * timeFrame;

			int a = coordinates(0) * 10;
			int b = round(coordinates(0)) * 10;

			if (a == b)
				direction = 6;
			break;
		}
		case LEFT:
		{
			coordinates(2) += 1 * timeFrame;

			int a = coordinates(2) * 10;
			int b = round(coordinates(2)) * 10;

			if (a == b)
				direction = 6;
			break;
		}
		case RIGHT:
		{
			coordinates(2) -= 1 * timeFrame;

			int a = coordinates(2) * 10;
			int b = round(coordinates(2)) * 10;

			if (a == b)
				direction = 6;
			break;
		}
	}

	Mat4 m = obj->matrix;
	obj->matrix = Mat4::translate(coordinates) * m;
	glColor3f(1,1,0);
	obj->draw();
	obj->matrix = m;

}

void Pacman::drawHUD()
{


}

/*void Pacman::collsionCheck(vector<Ghost*> * ghosts, vector<Food*> food, Pacman * player, vector<Vec3> * terrain)
{


}*/
