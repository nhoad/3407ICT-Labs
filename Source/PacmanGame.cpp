#include "Loader.h"
#include "Pacman.h"

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

PacmanGame::~PacmanGame()
{
	delete pacman;
	delete terrain;
}

void PacmanGame::initialise()
{
	for (int i=0; i < 350; i++)
		keys[i] = false;

	loadShader("Assets/basic_shader_v.glsl", "Assets/basic_shader_f.glsl");

	setGameState(GAME_PLAY);
	cout << "Loading Pacman..." << endl;
	loadGhosts();
	loadFood();
	loadPlayer();
	loadMap();

	pacman->buffer();
}

void PacmanGame::update(double elapsedTime)
{
	switch (getGameState())
	{
		case GAME_QUIT:
			exit(0);
			break;
		case GAME_PAUSE:
			cerr << "can't pause yet" << endl;
			break;
		case GAME_MENU:
			cerr << "no menu yet" << endl;
			break;
	}

	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT:
				setGameState(GAME_QUIT);
				break;

			case SDL_KEYUP:
				keys[e.key.keysym.sym] = false;
				break;

			case SDL_KEYDOWN:
				keys[e.key.keysym.sym] = true;
				break;
		}
	}

	if (keys[SDLK_UP] || keys[SDLK_w])
		pacman->move(UP, elapsedTime);

	if (keys[SDLK_DOWN] || keys[SDLK_s])
		pacman->move(DOWN, elapsedTime);

	if (keys[SDLK_LEFT] || keys[SDLK_a])
		pacman->move(LEFT, elapsedTime);

	if (keys[SDLK_RIGHT] || keys[SDLK_d])
		pacman->move(RIGHT, elapsedTime);

	if (keys[SDLK_ESCAPE])
		setGameState(GAME_QUIT);

	for (int i=0; i < food_entities.size(); i++)
	{
		if (food_entities[i]->visible && pacman->collidesWith(food_entities[i]))
		{
			food_entities[i]->visible = false;
			score += food_entities[i]->points;
		}
	}
}

void PacmanGame::draw()
{
	glUseProgram(shader);
	Game::draw();

	for (int i=0; i < food_entities.size(); i++)
	{
		if (food_entities[i]->visible)
			food_entities[i]->draw();
	}

	pacman->draw();
	glUseProgram(0);
	terrain->draw();
}

void PacmanGame::loadMap()
{
	terrain = Loader::loadTerrain("Assets/pacman.png", 12);
}

void PacmanGame::loadGhosts()
{
	cerr << "I don't load any Ghosts yet!" << endl;
}

void PacmanGame::loadFood()
{
	Mesh * mesh = Loader::readMesh("Assets/cone.obj");

	Mat4 * matrix = new Mat4(Mat4::scale(28, 28, 28));

	unsigned int texture = Loader::loadTexture("Assets/Checkerboard.png");
	unsigned int vbo = Loader::buffer(mesh);

	for (int i=0; i < 13; i++)
	{
		addFood(new Food(10, mesh, matrix, new Vec3(2.3, 0, i+18), vbo, texture));
		addFood(new Food(10, mesh, matrix, new Vec3(2.3, 0, i+2.4), vbo, texture));
	}


	for (int i=0; i < 25; i++)
	{
		if (i == 2)
			i++;
		else if (i == 17)
			i += 2;

		addFood(new Food(10, mesh, matrix, new Vec3(i + 3.8, 0, 7.4), vbo, texture));
		addFood(new Food(10, mesh, matrix, new Vec3(i + 3.8, 0, 24.5), vbo, texture));
	}

	for (int i=0; i < 28; i++)
	{
		addFood(new Food(10, mesh, matrix, new Vec3(5.7, 0, i+2.4), vbo, texture));
		addFood(new Food(10, mesh, matrix, new Vec3(21.2, 0, i+2.4), vbo, texture));
		addFood(new Food(10, mesh, matrix, new Vec3(31.5, 0, i+2.4), vbo, texture));
	}
}

void PacmanGame::addFood(Food * food)
{
	food_entities.push_back(food);
}

void PacmanGame::loadPlayer()
{
	this->pacman = new Pacman();
	this->lives = 3;
}
