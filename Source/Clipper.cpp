#include "Clipper.h"

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

vector<Vertex> Clipper::clipLeft(vector<Vertex> polygon)
{
	vector<Vertex> result;

	const int minX = 0;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Vertex a = polygon[i];
		Vertex b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		// if the vertex is invisible, ignore it.
		if (a(0) < minX && b(0) < minX)
			continue;
		// if both are visible, add the first one.
		else if (a(0) >= minX && b(0) >= minX)
			result.push_back(a);
		// else, deal with a partially visible line.
		else
		{
			double dy = b(1) - a(1);
			double dx = b(0) - a(0);

			// if a == in and b == out
			if (a(0) >= minX)
			{
				b(1) += ((double)(dy / dx)) * (double)(minX - b(0));
				b(0) = minX;

				result.insert(result.begin(), b);
				result.insert(result.begin(), a);
			}
			else // a == out and b == in
			{
				a(1) += ((double)(dy / dx)) * (double)(minX - a(0));
				double oldX = a(0);
				a(0) = minX;

				result.insert(result.begin(), a);
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
			double dy = b(1) - a(1);
			double dx = b(0) - a(0);

			// if a == in and b == out
			if (a(0) <= maxX)
			{
				b(1) += (dy / dx) * (maxX - b(0));
				b(0) = maxX-1;

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				a(1) += (dy / dx) * (maxX - a(0));
				double oldX = a(0);
				a(0) = maxX-1;

				float r_inc = (b(4) - a(4)) / dx;
				float g_inc = (b(5) - a(5)) / dx;
				float b_inc = (b(6) - a(6)) / dx;

				cout << b(4) - a(4) << endl;
				cout << dy << endl;

				a(4) = r_inc * (a(0) - oldX);
				a(5) = g_inc * (a(0) - oldX);
				a(6) = b_inc * (a(0) - oldX);
				cout << a << endl;

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
			double dy = b(1) - a(1);
			double dx = b(0) - a(0);

			// if a == in and b == out
			if (a(1) < maxY)
			{
				b(0) += (maxY - b(1)) / (dy / dx);
				b(1) = maxY-1;

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				a(0) += (maxY - a(1)) / (dy / dx);
				a(1) = maxY-1;

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
		if (a(1) < minY && b(1) < minY)
			continue;
		else if (a(1) >= minY && b(1) >= minY)
			result.push_back(a);
		else
		{
			double dy = b(1) - a(1);
			double dx = b(0) - a(0);

			// if a == in and b == out
			if (a(1) >= minY)
			{
				b(0) += (minY - b(1)) / (dy / dx);
				b(1) = minY;

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				a(0) += (minY - a(1)) / (dy / dx);
				a(1) = minY;

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


