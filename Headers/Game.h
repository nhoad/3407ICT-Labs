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

#define GAME_PLAY 0
#define GAME_QUIT 1
#define GAME_PAUSE 2
#define GAME_MENU 3

class GameEntity;

/**
 * "Game" class representing a game, containing all the entities and player, shader, etc.
 * All methods do nothing unless otherwise stated
 */
class Game
{
	protected:
		unsigned int shader;
	private:
		std::vector<GameEntity *> entities;
		int gameState;

	public:
		/// Constructor. Does nothing, override initialise instead.
		Game();

		/**
		 * Destructor. Deletes all game entities, and the player. If you overload
		 * this make sure you call Game::~Game()
		 */
		virtual ~Game();

		/**
		 * Initialise the game. You should create your player and game entities here.
		 */
		virtual void initialise();

		/**
		 * Update the games parameters. This could be handling User input, moving objects, collisions, anything.
		 *
		 * \param elapsedTime the amount of time passed since the previous frame was drawn, in microseconds
		 */
		virtual void update(double elapsedTime);

		/**
		 * Draw the game. This method draws your GameEntities, so you should still call Game::draw();
		 */
		virtual void draw();

		/**
		 * Add an entity to the game.
		 * You should NOT add the player object. Sort that out in your own Game class!
		 *
		 * \param entity the new entity to add
		 */
		void addGameEntity(GameEntity * entity);

		/**
		 * links and compiles the vertex and fragment shaders for this game
		 *
		 * \param vertexShaderScript filepath to the vertex shader script to load
		 * \param fragmentShaderScript filepath to the vertex shader script to load
		 */
		unsigned int loadShader(std::string vertexShaderScript, std::string fragmentShaderScript);

		/**
		 * Set the game state to one of many states
		 * Supports GAME_QUIT, GAME_PAUSE, GAME_MENU, or GAME_PLAY.
		 */
		void setGameState(int state);

		/**
		 * Get the current game state
		 */
		int getGameState();
};

class GameEntity
{
	protected:
		Vec3 * coordinates;

	private:
		/**
			Returns the centre point for an axis.

			\param k the axis to get the centre point of, using either X_AXIS, Y_AXIS or Z_AXIS.
			\return centre point of axis k.
		*/
		virtual float centre(int k);

		Mesh * mesh;
		Mat4 * matrix;

		unsigned int vbo, texture;

	public:

		/**
		 * Constructor. Loads a mesh and texture from file. Mesh will NOT be loaded to a VBO.
		 *
		 * \param meshFile filepath to the mesh
		 * \param textureFile filepath to the texture
		 * \param m pointer to this objects transformation matrix
		 * \param startPosition the starting position of this object
		 */
		GameEntity(std::string meshFile, std::string textureFile, Mat4 * m, Vec3 * startPosition);

		/**
		 * Constructor. Takes a pointer to a preloaded mesh, transformation matrix, texture and start position
		 *
		 * \param mesh pointer to the mesh
		 * \param m pointer to the transformation matrix
		 * \param texture id of the texture to use
		 * \param startPosition Vec3 containing location coordinates
		 */
		GameEntity(Mesh * mesh, Mat4 * m, unsigned int texture, Vec3 * startPosition);

		/**
		 * Delete all items in this entity without deleting itself.
		 *
		 * Note that this leaves this entity UNDRAWABLE. Any objects sharing resources with this object will
		 * also be UNDRAWABLE.
		 */
		virtual void cleanUp();

		/// Destructor
		virtual ~GameEntity();

		/**
			return centre point of cube on the x axis.

			\return x axis centroid.
		*/
		virtual float centreX();

		/**
			return centre point of cube on the y axis.

			\return y axis centroid.
		*/
		virtual float centreY();

		/**
			return centre point of cube on the z axis.

			\return z axis centroid.
		*/
		virtual float centreZ();

		/**
		 * Draw this object. Makes appropriate glPush, glMult, various pointers and
		 * bindings, and calls glDrawArrays with a glPopMatrix to finish it all off.
		 * Unless you're doing something fancy, I wouldn't recommend overloading this
		 */
		virtual void draw();

		/**
		 * Send the mesh to a VBO
		 */
		virtual void buffer();

		/**
		 * Set the VBO. This is ONLY to be used when you're loading multiple objects
		 * that use the same mesh, and want to save space.
		 *
		 * \param vbo the new vbo identifier to set for this class
		 */
		virtual void setVBO(unsigned int vbo);

		/**
		 * Set the texture ID. This is ONLY to be used when you're loading the same
		 * texture onto multiple objects and want to save space.
		 *
		 * \param texture the new texture identifier to set for this class
		 */
		virtual void setTexture(unsigned int texture);

		/**
		 * Transform this objects current transformation matrix by m. You should NOT
		 * store the coordinates in this matrix, they're computed by draw() using the coordinates vector.
		 *
		 * Stores current_matrix * m
		 *
		 * \param m pointer to the transformation matrix to multiply current matrix by.
		 */
		virtual void transform(Mat4 * m);

		/**
		 * Reset the transformation matrix to an identity matrix
		 */
		virtual void loadIdentity();

		/**
		   Move the current object by these amounts

			\param x_amount the amount to move this object on the x axis.
			\param y_amount the amount to move this object on the y axis.
			\param z_amount the amount to move this object on the z axis.
		*/
		virtual void move(double x_amount, double y_amount, double z_amount);

		virtual Vec3 getCoordinates();
};
