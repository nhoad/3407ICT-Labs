#include "Clipper.h"

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

#include <cmath>


vector<Vertex> Clipper::clipLeft(vector<Vertex> polygon)
{
	vector<Vertex> result;
	const int minX = 0;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Vertex a = polygon[i];
		Vertex b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		// if the vertex is invisible, ignore it.
		if ((round(a(0)) < minX) && (round(b(0)) < minX))
			continue;
		// if both are visible, add the first one.
		else if (round(a(0)) >= minX && round(b(0)) >= minX)
		{
			cout << "adding a" << endl;
			result.push_back(a);
		}
		// else, deal with a partially visible line.
		else
		{
			float dy = b(1) - a(1);
			float dx = b(0) - a(0);

			// if a == in and b == out
			if (round(a(0)) >= minX)
			{
				b(1) += (dy / dx) * (minX - b(0));
				b(0) = minX;

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				a(1) += ((dy / dx)) * (minX - a(0));
				a(0) = minX;

				result.push_back(a);
				result.push_back(b);
			}
		}

	}

	return result;
}


vector<Vertex> Clipper::clipRight(vector<Vertex> polygon, const int maxX)
{
	vector<Vertex> result;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Vertex a = polygon[i];
		Vertex b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		if (a(0) > maxX && b(0) > maxX)
			continue;
		else if (a(0) <= maxX && b(0) <= maxX)
			result.push_back(a);
		else
		{
			float dy = b(1) - a(1);
			float dx = b(0) - a(0);

			// if a == in and b == out
			if (a(0) <= maxX)
			{
				float oldX = b(0), oldY = b(1);
				b(1) += (dy / dx) * (maxX - b(0));
				b(0) = maxX-1;

				float r_inc = (b(4) - a(4)) / dx;
				float g_inc = (b(5) - a(5)) / dx;
				float b_inc = (b(6) - a(6)) / dx;

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				a(1) += (dy / dx) * (maxX - a(0));
				a(0) = maxX-1;

				result.push_back(a);
			}
		}

	}

	return result;
}

vector<Vertex> Clipper::clipBottom(vector<Vertex> polygon, const int maxY)
{
	vector<Vertex> result;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Vertex a = polygon[i];
		Vertex b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		// if the vertex is invisible, ignore it.
		if (a(1) > maxY && b(1) > maxY)
			continue;
		else if (a(1) <= maxY && b(1) <= maxY)
			result.push_back(a);
		else
		{
			float dy = b(1) - a(1);
			float dx = b(0) - a(0);

			float r_inc = (b(4) - a(4)) / dy;
			float g_inc = (b(5) - a(5)) / dy;
			float b_inc = (b(6) - a(6)) / dy;

			// if a == in and b == out
			if (a(1) < maxY)
			{
				float oldY = b(1);
				b(0) += (maxY - b(1)) / (dy / dx);
				b(1) = maxY-1;

				b(4) += r_inc * (b(1) - oldY);
				b(5) += g_inc * (b(1) - oldY);
				b(6) += b_inc * (b(1) - oldY);

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				float oldY = a(1);
				a(0) += (maxY - a(1)) / (dy / dx);
				a(1) = maxY-1;

				a(4) += r_inc * (a(1) - oldY);
				a(5) += g_inc * (a(1) - oldY);
				a(6) += b_inc * (a(1) - oldY);

				result.push_back(a);
			}
		}

	}

	return result;
}

vector<Vertex> Clipper::clipTop(vector<Vertex> polygon)
{
	vector<Vertex> result;
	const int minY = 0;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Vertex a = polygon[i];
		Vertex b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		// if the vertex is invisible, ignore it.
		if (round(a(1)) < minY && round(b(1)) < minY)
			continue;
		else if (a(1) >= minY && b(1) >= minY)
			result.push_back(a);
		else
		{
			float dy = b(1) - a(1);
			float dx = b(0) - a(0);

			float r_inc = (b(4) - a(4)) / dy;
			float g_inc = (b(5) - a(5)) / dy;
			float b_inc = (b(6) - a(6)) / dy;

			// if a == in and b == out
			if (a(1) >= minY)
			{
				float oldY = b(1);

				b(0) += (minY - b(1)) / (dy / dx);
				b(1) = minY;

				b(4) += r_inc * (b(1) - oldY);
				b(5) += g_inc * (b(1) - oldY);
				b(6) += b_inc * (b(1) - oldY);

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				float oldY = a(1);

				a(0) += (minY - a(1)) / (dy / dx);
				a(1) = minY;

				a(4) += r_inc * (a(1) - oldY);
				a(5) += g_inc * (a(1) - oldY);
				a(6) += b_inc * (a(1) - oldY);

				result.push_back(a);
			}
		}

	}

	return result;
}

vector<Vertex> Clipper::clip(vector<Vertex> polygon, const int width, const int height)
{
	polygon = clipTop(polygon);
	polygon = clipLeft(polygon);
	polygon = clipRight(polygon, width);
	polygon = clipBottom(polygon, height);

	return polygon;
}


