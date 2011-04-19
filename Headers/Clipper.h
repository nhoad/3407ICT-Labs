#include "Primitives.h"

class Clipper
{
	/**
		Perform clipping for the left side of the viewport.

		\param polygon the polygon to clip.
		\return the clipped polygon.
	*/
	static std::vector<Vertex> clipLeft(std::vector<Vertex> polygon);

	/**
		Perform clipping for the top side of the viewport.

		\param polygon the polygon to clip.
		\return the clipped polygon.
	*/
	static std::vector<Vertex> clipTop(std::vector<Vertex> polygon);

	/**
		Perform clipping for the bottom side of the viewport.

		\param polygon the polygon to clip.
		\return the clipped polygon.
	*/

	static std::vector<Vertex> clipBottom(std::vector<Vertex> polygon, const int maxY);
	/**
		Perform clipping for the right side of the viewport.

		\param polygon the polygon to clip.
		\return the clipped polygon.
	*/
	static std::vector<Vertex> clipRight(std::vector<Vertex> polygon, const int maxX);

	public:
	/**
		Perform clipping for all sides of the view port.

		\param polygon the polygon to clip.
		\param width the width of the viewport
		\param height the height of the viewport.
		\return description
	*/
	static std::vector<Vertex> clip(std::vector<Vertex> polygon, const int width, const int height);

};
