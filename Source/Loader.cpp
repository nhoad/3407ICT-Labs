/*e
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#pragma once

#include "glew.h"

#include <string>
using std::string;

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;

#include <cerrno>
#include <cstring>
#include <cstdlib>

#include "StringFunctions.h"
#include "Loader.h"
#include "Vec2.h"
#include "Vec3.h"

Mesh Loader::readMesh(const string filename)
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

	vector<Vec2> texture_coords;
	vector<Vec3> normals, vertices;

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
			vertices.push_back(Vec3(x, y, z));
		}
		else if (type == "vn")
		{
			x = stringToType<float>(split_line[1]);
			y = stringToType<float>(split_line[2]);
			z = stringToType<float>(split_line[3]);

			normals.push_back(Vec3(x, y, z));
		}
		else if (type == "vt")
		{
			x = stringToType<float>(split_line[1]);
			y = stringToType<float>(split_line[2]);

			texture_coords.push_back(Vec2(x, y));
		}
		else if (type == "f")
		{
			face_sizes.push_back(split_line.size() - 1);

			for (unsigned i=1; i < split_line.size(); i++)
			{
				vector<string> split_face = tokenize(split_line[i], "/");
				int face_number = stringToType<int>(split_face[0]);
				int texture_number = stringToType<int>(split_face[1]);
				int normal_number = stringToType<int>(split_face[2]);

				add_order.push_back(face_number);
				add_order.push_back(texture_number);
				add_order.push_back(normal_number);
			}
		}

	}

	cout << "Loaded " << vertices.size() << " vertices" << endl;
	cout << "Loaded " << texture_coords.size() << " texture coordinates" << endl;
	cout << "Loaded " << normals.size() << " normals" << endl;

	for (int i=0; i < add_order.size(); i+= 3)
	{
		float x, y, z, tx, ty, nx, ny, nz;

		x = vertices[add_order[i]-1](0);
		y = vertices[add_order[i]-1](1);
		z = vertices[add_order[i]-1](2);

		tx = texture_coords[add_order[i+1]-1](0);
		ty = texture_coords[add_order[i+1]-1](1);

		nx = normals[add_order[i+2]-1](0);
		ny = normals[add_order[i+2]-1](1);
		nz = normals[add_order[i+2]-1](2);

		mesh.push_back(Vertex(x, y, z, nx, ny, nz, tx, ty));
	}

	return mesh;
}

string Loader::readGLSL(string filename)
{
	string script = "";

	cout << "Loading GLSL script: " << filename << endl;

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

			lines.push_back(line);
		}
	}
	else
	{
		cout << "Loader: Could not open " << filename << ": " << strerror(errno) << endl;
		exit(1);
	}

	for (int i=0; i < lines.size(); i++)
		script += lines[i] + "\n";

	return script;
}

unsigned int Loader::loadShader(string scriptFile, int shaderType)
{
	string script = Loader::readGLSL(scriptFile);

	cout << "ShaderLoader: Loading Script: " << scriptFile << endl;

	unsigned int id = glCreateShader(shaderType);

	int result;
	int size = script.size();
	const char * script_c = script.c_str();

	glShaderSource(id, 1, &script_c, &size);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &result);
		char * errorMessage = new char[result];

		glGetShaderInfoLog(id, result, &result, errorMessage);

		cerr << "ShaderLoader: Could not compile " << scriptFile << ": " << endl << errorMessage << endl;

		delete errorMessage;
		glDeleteShader(id);
		return -1;
	}

	return id;
}

unsigned int Loader::linkShader(unsigned int vertex, unsigned int fragment)
{
	unsigned int id = glCreateProgram();

	glAttachShader(id, vertex);
	glAttachShader(id, fragment);

	glLinkProgram(id);

	int result;
	glGetShaderiv(id, GL_LINK_STATUS, &result);

	if (result == GL_FALSE)
	{
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &result);
		char * errorMessage = new char[result];

		glGetShaderInfoLog(id, result, &result, errorMessage);

		cerr << "ShaderLoader: Could not compile script " << errorMessage << endl;

		delete errorMessage;
		glDeleteShader(id);
		return 0;
	}

	//glValidateProgram(?);

	return id;
}
