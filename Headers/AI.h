#ifndef AI_h
#define AI_h

#include <queue>

#include <list>

#include "Vec3.h"
#include "Game.h"
#include "Terrain.h"

class TreeNode;

/**
	Custom tree node class for tree search
*/
class TreeNode
{
	public:
		Vec3 coords;
		std::list<TreeNode*> children;
		float path_cost, heuristic;

		TreeNode * parent;
		TreeNode(Vec3 coords, TreeNode * parent, float path_cost, float heuristic) : coords(coords), parent(parent), path_cost(path_cost), heuristic(heuristic)
		{}

		~TreeNode();

};

/**
	AI class providing some AI functions
*/
class AI
{
	private:
		/**
			Manhattan distance heuristic function

			\return manhattan distance between two GameEntitys, in 2D only.
		*/
		float manhattanDistance(GameEntity * start, GameEntity * goal);

		void expand(TreeNode * n, std::priority_queue<TreeNode> * queue, Terrain * terrain);

		void addPosition(float x, float y, TreeNode * n);

	public:
		/**
			Perform a star search

			\param start the start node
			\param goal the goal node
			\param terrain the terrain. Used for collision detection against walls

			\return a Vec3 instructing what directions it should take, in x, y and z format. +1 means go positively along that axis, -1 means go negatively.
		*/
		static Vec3 aStar(GameEntity * start, GameEntity * goal, Terrain * terrain);
};

#endif
