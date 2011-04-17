/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Xavier Ho (contact@xavierho.com)
 */
#pragma once
#include "SDL.h"
#include "SDL_ttf.h"

/**
 * Top-tier class, handles mainloop, events, and other classes.
 * This class has been designed with a minimal style in mind.
 */
#include "Primitives.h"
#include "Transformation.h"

#include "SoftwareRendering.h"

class Assignment1 : public Core
{
	float angleX, angleY, angleZ;
	float yInc, xInc;

	Cube cube;

	Mat4 * projection;

	bool increaseScale, decreaseScale, dynamic, rotateX, rotateY, rotateZ;

	TTF_Font * font;

	public:
	virtual ~Assignment1();

	protected:

	/** Prepares objects for rendering. */
	void preprocess();

	/** Draws to screen. */
	void render();

	/** Handles user events. */
	void handleEvents();

	/**
	  draw a cube.

	  \param cube the cube to draw.
	  */
	void drawCube(Cube cube);

	/**
	  move a cube according to the global values stored in Assignment1.

	  \param cube the cube to move.
	  */
	void moveCube(Cube & cube);

	/**
	  draw a polygon

	  \param polygon the polygon to draw.
	  */
	void drawPolygon(Face polygon);

	/**
	  Decompose a polygon into triangles.

	  \param polygon the polygon to decompose.
	  \return returns a vector of Vertexs that will be a multiple of three.
	  */
	std::vector<Vertex> decompose(std::vector<Vertex> polygon);

	/**
	  create a line between two vertices.
	  \param a the first vertice to draw from
	  \param b the second vertice to draw to.
	  */
	std::vector<Vertex> makeLine(Vertex a, Vertex b);

	/**
	  scan line algorithm for polygon filling.
	  \param a the first point to draw from
	  \param b the second point to draw to.
	  */
	void scanLine(Vertex a, Vertex b);

	/**
	  draw a 2D triangle.
	  \param a point to draw triangle from
	  \param b intermediary point to draw from/to.
	  \param c point to draw to.
	  */
	void triangle(Vertex a, Vertex b, Vertex c);

	void drawText(const char * text, int x, int y);

};
