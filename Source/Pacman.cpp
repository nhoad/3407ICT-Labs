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
	// slam the mesh to the vbo

	// we use the players vbo because they're both the same object
	//unsigned int buffer = player->vbo;

	unsigned buffer;
	for (int i=0; i < count; i++)
	{
		Food * f = new Food(m, 10);

		if (i % 10 == 0)
			f->points = 20;

		// this needs to be modified to put them in the correct place
		f->obj->matrix = Mat4::translate(0, 1, 1);
		f->obj->vbo = buffer;

		food.push_back(f);
	}

	return food;
}

GameEntity* Pacman::loadPlayer(std::string meshFile, std::string textureFile)
{
	GameEntity * player = new GameEntity(meshFile, textureFile, Mat4::translate(0, 0, 0));

	glGenBuffers(1, &player->vbo);

	glBindBuffer(GL_ARRAY_BUFFER, player->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * player->mesh.size(),
		&player->mesh[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return player;
}

Pacman::Pacman()
{
	obj = Pacman::loadPlayer("Assets/Cube.obj", "");
	coordinates = Vec3(14, 1, 13);

	obj->matrix = Mat4::scale(20, 20, 20);

	/*Pacman::loadGhosts("Assets/Ghost.obj", "Assets/Ghost.png", 4);
	Pacman::loadFood("Assets/Ball.obj", 50);*/
}

void Pacman::move(int direction)
{
	switch (direction)
	{
		case UP:
		{
			coordinates(0)--;
			break;
		}
		case DOWN:
		{
			coordinates(0)++;
			break;
		}
		case LEFT:
		{
			coordinates(2)++;
			break;
		}
		case RIGHT:
		{
			coordinates(2)--;
			break;
		}
	}
}

void Pacman::draw()
{
	Mat4 m = obj->matrix;
	obj->matrix = Mat4::translate(coordinates) * m;
	obj->draw();
	obj->matrix = m;

}

void Pacman::drawHUD()
{


}
