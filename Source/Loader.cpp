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

#include "SDL_image.h"

#include "StringFunctions.h"
#include "Loader.h"
#include "Vec2.h"
#include "Vec3.h"

Mesh * Loader::readMesh(const string filename)
{
	Mesh * mesh = new Mesh;

	cout << "Loading model: " << filename << endl;
	mesh->clear();

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

		mesh->push_back(Vertex(x, y, z, nx, ny, nz, tx, ty));
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

unsigned int Loader::loadTexture(std::string file)
{
	SDL_Surface * img = IMG_Load(file.c_str());

	unsigned int result;

	glGenTextures(1, &result);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, img->format->BytesPerPixel, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(img);

	return result;
}

Vec3 Loader::getPixel(SDL_Surface * img, int x, int y)
{
	float r, g, b;
	Uint8 * p = (Uint8 *)img->pixels + y * img->pitch + x * img->format->BytesPerPixel;
#if defined(__MACH__) && defined(__APPLE__)
	b = p[0];
	g = p[1];
	r = p[2];
#else
	b = p[2];
	g = p[1];
	r = p[0];
#endif

	return Vec3(r, g, b);
}

Terrain * Loader::loadTerrain(string heightmap, float divisions)
{
	SDL_Surface * img = IMG_Load(heightmap.c_str());

	float xDiv = divisions, zDiv = xDiv;
	unsigned int vbo, color_vbo;

	vector<float> heights;
	vector<Vec3> polygon, colors;

	SDL_LockSurface(img);

	int width = img->w, depth = img->h;

	for (int i=0; i < img->w; i++)
		for (int j=0; j < img->h; j++)
		{
			Vec3 color = Loader::getPixel(img, i, j);
			heights.push_back(color(0) / (divisions / 4));
		}

	SDL_UnlockSurface(img);
	SDL_FreeSurface(img);

	for (int z=0; z < width -1; z++)
		for (int x=0; x < depth -1; x++)
		{
			int curX = x * xDiv, curZ = z * zDiv;

			polygon.push_back(Vec3(curX, Terrain::getHeight(&heights, x, z, width), curZ));
			polygon.push_back(Vec3(curX+xDiv, Terrain::getHeight(&heights, x+1, z+1, width), curZ+zDiv));
			polygon.push_back(Vec3(curX, Terrain::getHeight(&heights, x, z+1, width), curZ+zDiv));

			polygon.push_back(Vec3(curX, Terrain::getHeight(&heights, x, z, width), curZ));
			polygon.push_back(Vec3(curX+xDiv, Terrain::getHeight(&heights, x+1, z, width), curZ));
			polygon.push_back(Vec3(curX+xDiv, Terrain::getHeight(&heights, x+1, z+1, width), curZ+zDiv));

			colors.push_back(Terrain::getColour(&heights, x, z, width));
			colors.push_back(Terrain::getColour(&heights, x+1, z+1, width));
			colors.push_back(Terrain::getColour(&heights, x, z+1, width));

			colors.push_back(Terrain::getColour(&heights, x, z, width));
			colors.push_back(Terrain::getColour(&heights, x+1, z, width));
			colors.push_back(Terrain::getColour(&heights, x+1, z+1, width));
		}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, polygon.size() * sizeof(Vec3), &polygon[0](0), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, polygon.size() * sizeof(Vec3), &colors[0](0), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Terrain * t = new Terrain(vbo, color_vbo, colors.size());

	return t;
}
