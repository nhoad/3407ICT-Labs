/**
 * Game classes.
 * Author: Nathan Hoad.
 * Date: 20th May 2011
 *
 * Description: This is a base set of classes use for a game engine.
 */
#pragma once

#include "Mat4.h"
#include "Primitives.h"
#include "Vec3.h"

#include <vector>

class Game
{
	private:
		std::vector<GameEntity *> gameEntities;
		GameEntity * player;
		int shader;

	public:
		/**
		 * Initialise the game. You should create your palyer and game entities here.
		 */
		virtual void initialise();

		/**
		 * Update the games parameters. This could be handling User input, moving objects, collisions, anything.
		 */
		virtual void update();

		/**
		  draw the Heads up display to the user.
		*/
		virtual void drawHUD();

		/**
		 * Draw all of the game entities, and the player
		 */
		virtual void drawGameEntities();

		/**
		 * links and compiles the vertex and fragment shaders for this game
		 *
		 * \param vertexShaderScript filepath to the vertex shader script to load
		 * \param fragmentShaderScript filepath to the vertex shader script to load
		 */
		void loadShader(std::string vertexShaderScript, std::string fragmentShaderScript);
};

class GameEntity
{
	private:
		/**
			Returns the centre point for an axis.

			\param k the axis to get the centre point of, using either X_AXIS, Y_AXIS or Z_AXIS.
			\return centre point of axis k.
		*/
		float centre(int k);

		Mesh * mesh;
		Mat4 * matrix;
		Vec3 * coordinates;

		unsigned int vbo, texture;
	public:

		/**
		 * Constructor. Loads a mesh and texture from file, and buffers them.
		 *
		 * \param meshFile filepath to the mesh
		 * \param textureFile filepath to the texture
		 * \param m pointer to the matrix
		 */
		GameEntity(std::string meshFile= "", std::string textureFile = "", Mat4 * m);

		/**
		 * Constructor. Takes a pointer to a preloaded mesh and preloaded transformation matrix
		 */
		GameEntity(Mesh * mesh, Mat4 * m);

		/// Destructor
		virtual ~GameEntity();

		/**
			return centre point of cube on the x axis.

			\return x axis centroid.
		*/
		float centreX();

		/**
			return centre point of cube on the y axis.

			\return y axis centroid.
		*/
		float centreY();

		/**
			return centre point of cube on the z axis.

			\return z axis centroid.
		*/
		float centreZ();

		/**
		 * Make openGL calls to draw this object
		 */
		void draw();

		/**
		 * Send the mesh to the VBO and set vbo variable accordingly
		 */
		void buffer();

		/**
		 * Set the VBO. This is ONLY to be used when you're loading multiple objects
		 * that use the same mesh, and want to save space.
		 *
		 * \param vbo the new vbo identifier to set for this class
		 */
		void setVBO(unsigned int vbo);
};
