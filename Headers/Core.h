/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Xavier Ho (contact@xavierho.com)
 */
#pragma once

#include "Primitives.h"

/**
 * Top-tier class, handles mainloop, events, and other classes.
 * This class has been designed with a minimal style in mind.
 */
class Core
{
	/** Width and height of the rendering window. */
	int width, height;

	/** fullscreen flag */
	bool fullscreen;

	/** The amount of time passed after each frame */
	double elapsedTime;

	/** buffer for drawing */
	SDL_Surface * buffer;

	/** Mainloop control toggle */
	bool running;

	/** The mesh for a cube */
	Cube cube;

	/** rotation angle */
	float angle;

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

	/**
		Performs clipping on a polygon.

		\param polygon the polygon to clip.
		\return returns a clipped polygon.
	*/
	std::vector<Point> clip(std::vector<Point> polygon);

	/**
		Performs clipping on left side of a polygon.

		\param polygon the polygon to clip.
		\return returns a left-side clipped polygon.
	*/
	std::vector<Point> clipLeft(std::vector<Point> polygon);

	/**
		Performs clipping on right side of a polygon.

		\param polygon the polygon to clip.
		\return returns a right-side clipped polygon.
	*/
	std::vector<Point> clipRight(std::vector<Point> polygon);

	/**
		Performs clipping on top side of a polygon.

		\param polygon the polygon to clip.
		\return returns a top-side clipped polygon.
	*/
	std::vector<Point> clipTop(std::vector<Point> polygon);

	/**
		Performs clipping on left side of a polygon.

		\param polygon the polygon to clip.
		\return returns a bottom-side clipped polygon.
	*/
	std::vector<Point> clipBottom(std::vector<Point> polygon);

	/**
		make a line from point A to point B.

		\param a the starting point
		\param b the finish point.
		\return a vector of Points, making up a line from A to B.
	*/
	std::vector<Point> makeLine(Point a, Point b);

	/**
	 draw a convex polygon.

	 \param polygon the polygon to draw.
	*/
	void drawPolygon(std::vector<Point> polygon);

	/**
	 Decompose a polygon into triangles.

	 \param polygon the polygon to decompose.
	 \return returns a vector of Points that will be a multiple of three.
	*/
	std::vector<Point> decompose(std::vector<Point> polygon);

	/**
	 scan line algorithm for polygon filling.
	 \param a the first point to draw from
	 \param b the second point to draw to.
	*/
	void scanLine(Point a, Point b);

	/**
	 draw a triangle.
	 \param a point to draw triangle from
	 \param b point to draw triangle from
	 \param c point to draw triangle from
	*/
	void triangle(Point a, Point b, Point c);

	/**
		Draw a cube.

		\param cube the cube to draw.
	*/
	void drawCube(Cube cube);

	/**
		move a cube.

		\param cube the cube to move, according to global acceleration values.
	*/
	void moveCube(Cube & cube);

	void putpixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
};
