#include "ObjectLoader.h"
#include <string>
using std::string;

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;

#include <cstring>

#include <iostream>
using std::cout;
using std::endl;

#include <sstream>
using std::istringstream;

string strip(string line)
{
	int trim_pos = line.find_first_not_of(" \n\t");

	if (trim_pos == string::npos)
		return line;

	line = line.substr(trim_pos, line.length() - trim_pos + 1);
	trim_pos = line.find_last_not_of(" \n\t");

	if (trim_pos == string::npos)
		return line;
	line = line.substr(0, trim_pos + 1);

	return line;
}

template <typename T> T stringToType(string text)
{
	istringstream converter(text);

	T result;
	converter >> result;
	return result;
}

vector<string> tokenize(const string text, const string pattern)
{
	vector<string> result;

	size_t lastPos = text.find_first_not_of(pattern, 0);
	size_t firstPos = text.find_first_of(pattern, lastPos);

	while (firstPos != string::npos)
	{
		result.push_back(text.substr(lastPos, firstPos - lastPos));
		lastPos = text.find_first_not_of(pattern, firstPos);
		firstPos = text.find_first_of(pattern, lastPos);
	}

	result.push_back(text.substr(lastPos, firstPos - lastPos));

	return result;
}

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
		cout << "Could not open file" << endl;
	}

	float x, y, z;

	Mesh vertices;

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

			// we don't want to add them to the mesh just yet.
			vertices.push_back(Vertex(x, y, z, 1.0, x, y, z, 1.0));
			cout << line << endl;
		}
		else if (type == "vn")
		{
			// add a normal?
		}
		else if (type == "f")
		{
			for (int i=1; i < split_line.size(); i++)
			{
				vector<string> split_face = tokenize(split_line[i], "/");
				int number = stringToType<int>(split_face[0]);

				add_order.push_back(number);
			}
		}

	}

	for (int i=0; i < add_order.size(); i++)
		mesh.push_back(vertices[add_order[i]-1]);

}
