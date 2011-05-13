/*e
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#pragma once

#include "glew.h"

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
using std::cerr;

#include <cerrno>
#include <cstring>
#include <cstdlib>

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
