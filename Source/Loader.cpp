/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#include "Loader.h"
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

Mesh Loader::read(const string filename)
{
	Mesh mesh;

	cout << "Loading model: " << filename << endl;
	mesh.clear();

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
		cout << "Loader: Could not open " << filename << ": " << strerror(errno) << endl;
		exit(1);
	}

	float x, y, z;

	vector<Vertex> vertices;

	vector<int> face_sizes;
	vector<int> add_order;

	for (int i=0; i < lines.size(); i++)
	{
		line = lines[i];

		vector<string> split_line = tokenize(line, " \t\n");

		const string type = split_line[0];

		if (type == "v")
		{
			x = stringToType<float>(split_line[1]);
			y = stringToType<float>(split_line[2]);
			z = stringToType<float>(split_line[3]);

			// add them in the order they were read in, so we can get the proper ordering later.
			vertices.push_back(Vertex(x, y, z));
		}
		else if (type == "vn")
		{
			// add a normal?
		}
		else if (type == "f")
		{
			face_sizes.push_back(split_line.size() - 1);

			for (unsigned i=1; i < split_line.size(); i++)
			{
				vector<string> split_face = tokenize(split_line[i], "/");
				int number = stringToType<int>(split_face[0]);

				add_order.push_back(number);
			}
		}

	}

	for (int j=0, lastFaceEndPos=0; j < face_sizes.size(); j++)
	{

		for (int i=lastFaceEndPos; i < face_sizes[j] + lastFaceEndPos; i++)
			mesh.push_back(vertices[add_order[i]-1]);
			//face.push_back(vertices[add_order[i]-1]);

		lastFaceEndPos += face_sizes[j];

	}

	return mesh;
}

string Loader::loadGLSL(string filename)
{
	string script = "";

	cout << "Loading GLSL script: " << filename << endl;

	ifstream file(name.c_str());

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
		cout << "Loader: Could not open " << filename << ": " << strerror(errno) << endl;
		exit(1);
	}

	for (int i=0; i < lines.size(); i++)
		script += lines[i];

	return script;
}
