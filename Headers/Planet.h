/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#pragma once
#include "SDL.h"
#include <vector>
#include "glew.h"

/**
 * Top-tier class, handles mainloop, events, and other classes.
 * This class has been designed with a minimal style in mind.
 */

class Core
{
	/** Width and height of the rendering window. */
	int width, height;

	unsigned int * buffers, bufferCount, * verticeCount;

	/** fullscreen flag */
	bool fullscreen;

	/** The amount of time passed after each frame */
	double elapsedTime;

	/** Mainloop control toggle */
	bool running;

	double angle;

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

	void planet(double r);
};
