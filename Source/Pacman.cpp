#include "Pacman.h"
#include "Loader.h"

#include <vector>
using std::vector;

#include <string>
using std::string;

Food::Food(Mesh mesh, int points)
{
	this->points = points;
	this->obj = new Object(mesh);
}


Ghost::Ghost(int color, Object * object)
{
	this->color = color;
	scared = false;
	visible = false;
	obj = object;
}

void Pacman::initialise()
{
	Pacman::loadGhosts("Assets/Ghost.obj", "Assets/Ghost.png", 4);
	Pacman::loadFood("Assets/Ball.obj", 50);
	Pacman::loadPlayer("Assets/Ball.obj", "Assets/Pacman.png");
}

void Pacman::loadGhosts(string meshFile, string textureFile, int count)
{
	Mesh mesh = Loader::readMesh(meshFile);

	for (int i=0; i < 4; i++)
		ghosts.push_back(new Ghost(i, new Object(mesh)));
}

void Pacman::loadFood(std::string meshFile, int count)
{
	Mesh m = Loader::readMesh(meshFile);
	// slam the mesh to the vbo

	for (int i=0; i < count; i++)
	{
		Food * f = new Food(m, 10);

		if (i % 10 == 0)
			f->points = 20;

		// this needs to be modified to put them in the correct place
		f->obj->matrix = Mat4::translate(0, 1, 1);

		Pacman::food.push_back(f);
	}
}

void Pacman::loadPlayer(std::string meshFile, std::string textureFile)
{
	player = new Object("Assets/ball.obj", "", Mat4::translate(0, 0, 0));
}
