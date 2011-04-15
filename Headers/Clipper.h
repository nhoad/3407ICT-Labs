#include "Primitives.h"

class Clipper
{
	static std::vector<Vertex> clipLeft(std::vector<Vertex> polygon);
	static std::vector<Vertex> clipTop(std::vector<Vertex> polygon);
	static std::vector<Vertex> clipBottom(std::vector<Vertex> polygon, const int maxY);
	static std::vector<Vertex> clipRight(std::vector<Vertex> polygon, const int maxX);

	public:
	static std::vector<Vertex> clip(std::vector<Vertex> polygon, const int width, const int height);

};
