#include "AI.h"

#include <cmath>

using std::priority_queue;

Vec3 aStar(GameEntity * start, GameEntity * goal, Terrain * terrain)
{
	Vec3 result;

	priority_queue<TreeNode> fringe;

	if (AI::manhattanDistance(start, goal) == 0)
	{
		return result;
	}

	TreeNode * initialState = new TreeNode(start->getWorldCoordinates(), 0, 0, AI::manhattanDistance(start, goal));
	TreeNode * current = 0;
	TreeNode * goalState = new TreeNode(goal, 0, 0, 0);

	AI::expand(initialState, &fringe, terrain);

	while (!fringe.empty())
	{
		current = fringe.poll();

		if (current == goalState)
		{
			TreeNode solution = getSolutionNode(currentNode);

			result.x = solution.x;
			result.y = solution.y;
			result.z = solution.z;

			return result;
		}
		else
			expand(current, &fringe);
	}

	return result;

	// memory leaks!
}

void AI::expand(TreeNode * n, priority_queue<TreeNode> * queue, Terrain * terrain)
{
	if (terrain->canGoRight(n->coords))
		addPosition(queue, n->coords.x - 2, n->coords.y, n);

	if (terrain->canGoLeft(n->coords))
		addPosition(queue, n->coords.x + 2, n->coords.y, n);

	if (terrain->canGoDown(n->coords))
		addPosition(queue, n->coords.x, n->coords.y + 2, n);

	if (terrain->canGoUp(n->coords))
		addPosition(queue, n->coords.x, n->coords.y - 2, n);
}

void AI::addPosition(priority_queue<TreeNode> * fringe, float x, float y, TreeNode * n)
{
	if (!checkLoop(x, y, n))
	{
		TreeNode * newNode = new TreeNode(x, y, n, n.path_cost() + 1, n.path_cost + 1 + AI::manhattanDistance(x, y, goal));
		node->children.add(newNode);
		fringe->add(newNode);
	}
}

float AI::manhattanDistance(GameEntity * start, GameEntity * goal)
{
	float a_x, a_y, b_x, b_y;

	Vec3 a_w = start->getWorldCoordinates();
	Vec3 b_w = goal->getWorldCoordinates();

	a_x = a_w.x;
	a_y = a_w.y;

	b_x = b_w.x;
	b_y = b_w.y;

	return abs(a_x - bx) + abs(a_x - b_x);
}

TreeNode::~TreeNode()
{
	for (int i=0; i < children; i++)
		delete children[i];
}
