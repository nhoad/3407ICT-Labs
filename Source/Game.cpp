#include "Game.h"
#include "Loader.h"

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;

#include "SDL_image.h"

Game::Game()
{
}

Game::~Game()
{
	// it's assumed that all objects in the entities
	if (entities.size() >= 1)
		entities[0]->deinitialise();

	for (int i=0; i < entities.size(); i++)
		delete entities[i];
}

void Game::initialise()
{

}

void Game::update(double elapsedTime)
{

}

unsigned int Game::loadShader(string vertexScript, string fragmentScript)
{
	unsigned int v = Loader::loadShader(vertexScript, GL_VERTEX_SHADER);
	unsigned int f = Loader::loadShader(fragmentScript, GL_FRAGMENT_SHADER);

	unsigned int result = Loader::linkShader(v, f);
	shader = result;

	glUseProgram(shader);

	float bg_colour[] = {0.1f, 0.2f, 0.4f, 0.0f};
	int bg_colour_id = glGetUniformLocation(shader, "bg_colour");

	glUniform3fv(bg_colour_id, 1, bg_colour);

	glUseProgram(0);
	cout << "using " << endl;

	return result;
}

void Game::draw()
{
	for (int i=0; i < entities.size(); i++)
		entities[i]->draw();
}

void Game::addGameEntity(GameEntity * entity)
{
	this->entities.push_back(entity);
}

void Game::setGameState(int state)
{
	this->gameState = state;
}

int Game::getGameState()
{
	return gameState;
}
