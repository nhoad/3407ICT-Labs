/**
 * 3407ICT Graphics Programming
 * ObjectLoader implementation.
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 */
#include "ObjectLoader.h"
#include <string>
using std::string;

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;

#include "StringFunctions.h"

#include <iostream>
using std::cout;
using std::endl;

#include <cerrno>
#include <cstring>
#include <cstdlib>

Mesh ObjectLoader::object()
{
	return mesh;
}

void ObjectLoader::read(const string filename)
{
	ifstream file(filename.c_str());

	vector<string> lines;
	string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);

			// remove whitespace from beginning and end of text.
			line = strip(line);

			// add all important lines
			if (line.size() > 0 && line.find("#") != 0)
				lines.push_back(line);
		}
	}
	else
	{
		cout << "ObjectLoader: Could not open " << filename << ": " << strerror(errno) << endl;
		exit(1);
	}

	float x, y, z;

	Mesh vertices;

	vector<int> add_order;

	for (unsigned i=0; i < lines.size(); i++)
	{
		line = lines[i];

		vector<string> split_line = tokenize(line, " \t\n");

		const string type = split_line[0];

		if (type == "v")
		{
			x = stringToType<float>(split_line[1]);
			y = stringToType<float>(split_line[2]);
			z = stringToType<float>(split_line[3]);

			// we don't want to add them to the mesh just yet.
			vertices.push_back(Vertex(x, y, z, 1.0, x, y, z, 1.0));
		}
		else if (type == "vn")
		{
			// add a normal?
		}
		else if (type == "f")
		{
			for (unsigned i=1; i < split_line.size(); i++)
			{
				vector<string> split_face = tokenize(split_line[i], "/");
				int number = stringToType<int>(split_face[0]);

				add_order.push_back(number);
			}
		}

	}

	for (unsigned i=0; i < add_order.size(); i++)
		mesh.push_back(vertices[add_order[i]-1]);

}
