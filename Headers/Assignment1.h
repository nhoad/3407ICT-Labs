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

/**
	First assignment. Renders a 3D obj that the user can interact with.
*/
class Assignment1 : public Core
{
	// maintain the rotations on x, y and z.
	float angleX, angleY, angleZ;

	// the amount the obj is moved each frame.
	float yInc, xInc, speed, scale;

	// z buffer
	float * zBuffer;

	// obj read from cube.obj
	std::vector<Object> objects;

	// font used to render text.
	TTF_Font * font;

	// the projection matrix.
	Mat4 * projection;

	// Path to the model file.
	std::string modelFile;

	// option to display wireframe only.
	bool wireframe;

	// display text for the users
	std::vector<std::string> instructions;

	// which transformations to perform.
	bool increaseScale, decreaseScale, dynamic, rotateX, rotateY, rotateZ;

	public:
	/** destructor*/
	virtual ~Assignment1();

	/**
		Set the path to the model to load.

		\param filename the file to load the object from
	*/
	void addModelFile(std::string filename);

	protected:

	/** Prepares objects for rendering. */
	void preprocess();

	/** Draws to screen. */
	void render();

	/** Handles user events. */
	void handleEvents();

	/**
	  draw a obj.

	  \param obj the cube to draw.
	  */
	void drawObject(Object obj);

	/**
	  move a obj according to the global values stored in Assignment1.

	  \param obj the cube to move.
	  */
	void moveObject(Object & obj);

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

	/**
		draw text on the screen.

		\param text the text to display.
		\param x x coord for text.
		\param y y coord for text
	*/
	void drawText(const char * text, int x, int y);

	/**
		Swap the colours of two edge lists.

		\param a the first edge list.
		\param b the second edge list.
	*/
	void colourSwap(std::vector<Vertex> &a, std::vector<Vertex> & b);

	/**
		load the instructions into the instruction and stats vectors
	*/
	void loadInstructions();

	/**
		Draw a line from point a to b. This is implemented with flat white, no colour interpolation at all.

		\param a starting point
		\param b end point
	*/
	void line(Vertex a, Vertex b);

};
