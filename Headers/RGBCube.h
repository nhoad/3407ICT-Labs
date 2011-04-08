/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Xavier Ho (contact@xavierho.com)
 */
#pragma once
#include "SDL.h"

/**
 * Top-tier class, handles mainloop, events, and other classes.
 * This class has been designed with a minimal style in mind.
 */
#include "Primitives.h"
class Core
{
	/** Width and height of the rendering window. */
	int width, height;

	/** fullscreen flag */
	bool fullscreen;

	/** The amount of time passed after each frame */
	double elapsedTime;

	float cameraX, cameraY, cameraZ;

	float angle;

	float cubeX, cubeY, yInc, xInc;

	Mesh cube;

	/** Mainloop control toggle */
	bool running;

	public:
	/** Constructor. */
	Core(int width=800, int height=600, bool fullscreen=false);

	/** Destructor */
	virtual ~Core();

	/** Starts the main loop. */
	void start();

	protected:
	/** Sets up rendering context. */
	void initialise();

	/** Prepares objects for rendering. */
	void preprocess();

	/** Draws to screen. */
	void render();

	/** Handles user events. */
	void handleEvents();

	void drawCube(int i_x, int i_y);

};
