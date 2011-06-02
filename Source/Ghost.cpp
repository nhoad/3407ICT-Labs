#include "Pacman.h"

Ghost::Ghost(Mesh * mesh, Mat4 * matrix, Vec3 * start, unsigned int vbo, unsigned int texture) :
	GameEntity(mesh, matrix, texture, start)
{
	setVBO(vbo);
	scared = false;
	visible = true;
}

Ghost::~Ghost() { }

Ghost::checkDirection(Pacman * pacman)
{
	int dx = 0, dy = 0;

	Vec3 direction = AI::aStar(this, pacman);

	// handle that shit!

	priority<TreeNode> fringe;

	if (heuristic(coordinates, pacman) == 0)
	{
		this->direction_x = 0;
		this->direction_z = 0;
		return;
	}

	// create the initial state
	TreeNode initialState(coordinates, 0, 0, heuristic(coordinates, pacman));

	// for simplicity we'll create a goal state
	TreeNode goalState(px, py, 0, 0, 0);

	expand(initialState, &fringe);

	TreeNode currentState;
	while (!fringe.empty())
	{
		currentState = fringe.poll();

		if (currentState == goalState)
		{
			TreeNode solution = getSolution(&currentState);

			move the ghosts
		}
		else
			expand(currentState, &fringe);
	}
}


