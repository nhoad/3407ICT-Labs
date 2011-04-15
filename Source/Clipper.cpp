vector<Point> Clipper::clipLeft(vector<Point> polygon)
{
	vector<Point> result;

	const int minX = 0;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		// if the vertex is invisible, ignore it.
		if (a.x < minX && b.x < minX)
			continue;
		// if both are visible, add the first one.
		else if (a.x >= minX && b.x >= minX)
			result.push_back(a);
		// else, deal with a partially visible line.
		else
		{
			double dy = b.y - a.y;
			double dx = b.x - a.x;

			// if a == in and b == out
			if (a.x >= minX)
			{
				b.y += ((double)(dy / dx)) * (double)(minX - b.x);
				b.x = minX;

				result.insert(result.begin(), b);
				result.insert(result.begin(), a);
			}
			else // a == out and b == in
			{
				a.y += ((double)(dy / dx)) * (double)(minX - a.x);
				double oldX = a.x;
				a.x = minX;

				double r_inc = (double) (b.r - a.r) / dy;
				double g_inc = (double) (b.g - a.g) / dy;
				double b_inc = (double) (b.b - a.b) / dy;

				a.r += r_inc * (oldX - a.x);
				a.g += g_inc * (oldX - a.x);
				a.b += b_inc * (oldX - a.x);

				result.insert(result.begin(), a);
			}
		}

	}

	return result;
}


vector<Point> Clipper::clipRight(vector<Point> polygon)
{
	vector<Point> result;
	const int maxX = width;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		if (a.x > maxX && b.x > maxX)
			continue;
		else if (a.x <= maxX && b.x <= maxX)
			result.push_back(a);
		else
		{
			double dy = b.y - a.y;
			double dx = b.x - a.x;

			// if a == in and b == out
			if (a.x <= maxX)
			{
				b.y += (dy / dx) * (maxX - b.x);
				b.x = maxX-1;

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				a.y += (dy / dx) * (maxX - a.x);
				a.x = maxX-1;

				result.push_back(a);
			}
		}

	}

	return result;
}

vector<Point> Clipper::clipBottom(vector<Point> polygon)
{
	vector<Point> result;
	const int maxY = height;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		// if the vertex is invisible, ignore it.
		if (a.y > maxY && b.y > maxY)
			continue;
		else if (a.y <= maxY && b.y <= maxY)
			result.push_back(a);
		else
		{
			double dy = b.y - a.y;
			double dx = b.x - a.x;

			// if a == in and b == out
			if (a.y < maxY)
			{
				b.x += (maxY - b.y) / (dy / dx);
				b.y = maxY-1;

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				a.x += (maxY - a.y) / (dy / dx);
				a.y = maxY-1;

				result.push_back(a);
			}
		}

	}

	return result;
}

vector<Point> Clipper::clipTop(vector<Point> polygon)
{
	vector<Point> result;
	const int minY = 0;

	for (unsigned i=0; i < polygon.size(); i++)
	{
		Point a = polygon[i];
		Point b = (i == polygon.size()-1) ? polygon[0] : polygon[i+1];

		// if the vertex is invisible, ignore it.
		if (a.y < minY && b.y < minY)
			continue;
		else if (a.y >= minY && b.y >= minY)
			result.push_back(a);
		else
		{
			double dy = b.y - a.y;
			double dx = b.x - a.x;

			// if a == in and b == out
			if (a.y >= minY)
			{
				b.x += (minY - b.y) / (dy / dx);
				b.y = minY;

				result.push_back(a);
				result.push_back(b);
			}
			else // a == out and b == in
			{
				a.x += (minY - a.y) / (dy / dx);
				a.y = minY;

				result.push_back(a);
			}
		}

	}

	return result;
}

vector<Point> Clipper::clip(vector<Point> polygon)
{
	polygon = clipTop(polygon);
	polygon = clipLeft(polygon);
	polygon = clipRight(polygon);
	polygon = clipBottom(polygon);

	return polygon;
}


