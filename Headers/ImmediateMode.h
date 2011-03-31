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
class Color
{
	public:
		Uint8 r, g, b;

		Color(Uint8 r=0, Uint8 g=0, Uint8 b=0) : r(r), g(g), b(b) {}
};

class Point
{
	public:
		int x, y;
		Color c;

		Point(int a, int b, Color c) : x(a), y(b), c(c) {}
};

class Core
{
	/** Width and height of the rendering window. */
	int width, height;

	/** fullscreen flag */
	bool fullscreen;

	/** The amount of time passed after each frame */
	double elapsedTime;

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

	void triangle(Point a, Point b, Point c);
};
