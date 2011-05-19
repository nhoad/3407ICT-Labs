#include "glew.h"

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
	Pacman::loadPlayer("Assets/Ball.obj", "Assets/Pacman.png");
	Pacman::loadFood("Assets/Ball.obj", 50);
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

	// we use the players vbo because they're both the same object
	unsigned int buffer = player->vbo;

	for (int i=0; i < count; i++)
	{
		Food * f = new Food(m, 10);

		if (i % 10 == 0)
			f->points = 20;

		// this needs to be modified to put them in the correct place
		f->obj->matrix = Mat4::translate(0, 1, 1);
		f->obj->vbo = buffer;

		Pacman::food.push_back(f);
	}
}

void Pacman::loadPlayer(std::string meshFile, std::string textureFile)
{
	player = new Object(meshFile, textureFile, Mat4::translate(0, 0, 0));

	glGenBuffers(1, &player->vbo);

	glBindBuffer(GL_ARRAY_BUFFER, player->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * player->mesh.size(),
		&player->mesh[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
