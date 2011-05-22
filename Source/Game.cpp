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
	for (int i=0; i < entities.size(); i++)
		delete entities[i];
}

void Game::initialise()
{

}

void Game::update(double elapsedTime)
{

}

void Game::loadShader(string vertexScript, string fragmentScript)
{
	unsigned int v = Loader::loadShader(vertexScript, GL_VERTEX_SHADER);
	unsigned int f = Loader::loadShader(fragmentScript, GL_FRAGMENT_SHADER);

	shader = Loader::linkShader(v, f);

	glUseProgram(shader);

	float bg_colour[] = {0.1f, 0.2f, 0.4f, 0.0f};
	int bg_colour_id = glGetUniformLocation(shader, "bg_colour");

	glUniform3fv(bg_colour_id, 1, bg_colour);
	glClearColor(bg_colour[0], bg_colour[1], bg_colour[2], bg_colour[3]);

	glUseProgram(0);
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
