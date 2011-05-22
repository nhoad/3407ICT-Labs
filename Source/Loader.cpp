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
#include <cassert>
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

	vector<int> face_sizes, v_add_order, vn_add_order, vt_add_order;

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
			mesh->useNormals();
			x = stringToType<float>(split_line[1]);
			y = stringToType<float>(split_line[2]);
			z = stringToType<float>(split_line[3]);

			normals.push_back(Vec3(x, y, z));
		}
		else if (type == "vt")
		{
			mesh->useTextureCoords();
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

				// add the face
				v_add_order.push_back(stringToType<int>(split_face[0]));

				// add the texture
				if (mesh->textures())
					vt_add_order.push_back(stringToType<int>(split_face[1]));

				// add the normal
				if (mesh->normals())
				{
					if (mesh->textures())
						vn_add_order.push_back(stringToType<int>(split_face[2]));
					else
						vn_add_order.push_back(stringToType<int>(split_face[1]));
				}
			}
		}
	}

	cout << "Loaded " << vertices.size() << " vertices" << endl;
	cout << "Loaded " << texture_coords.size() << " texture coordinates" << endl;
	cout << "Loaded " << normals.size() << " normals" << endl;

	for (int j=0, lastFaceEndPos=0; j < face_sizes.size(); j++)
	{
		vector<Vertex> face;

		for (int i=lastFaceEndPos; i < face_sizes[j] + lastFaceEndPos; i++)
		{
			Vec3 coords = vertices[v_add_order[i]-1];
			Vec3 normal;
			Vec2 tex;

			if (mesh->textures())
				tex = texture_coords[vt_add_order[i]-1];

			if (mesh->normals())
				normal = normals[vn_add_order[i]-1];

			face.push_back(Vertex(coords, normal, tex));
		}

		lastFaceEndPos += face_sizes[j];

		// decompose to triangles
		if (face.size() > 3)
		{
			vector<Vertex> decomposed;

			for (int i=1, max = face.size() - 1; i < max; i++)
			{
				decomposed.push_back(face[0]);
				decomposed.push_back(face[i]);
				decomposed.push_back(face[i+1]);
			}

			for (int i=0; i < decomposed.size(); i++)
				mesh->push_back(decomposed[i]);
		}
		else
			for (int i=0; i < face.size(); i++)
				mesh->push_back(face[i]);
	}

	cout << "final mesh size: " << mesh->size() << endl;
/*
	for (int i=0; i < add_order.size(); i+= 3)
	{
		float x, y, z, tx, ty, nx, ny, nz;

		x = vertices[add_order[i]-1](0);
		y = vertices[add_order[i]-1](1);
		z = vertices[add_order[i]-1](2);

		if (mesh->textures())
		{
			tx = texture_coords[add_order[i+1]-1](0);
			ty = texture_coords[add_order[i+1]-1](1);
		}

		if (mesh->normals())
		{
			nx = normals[add_order[i+2]-1](0);
			ny = normals[add_order[i+2]-1](1);
			nz = normals[add_order[i+2]-1](2);
		}

		mesh->push_back(Vertex(x, y, z, nx, ny, nz, tx, ty));
	}
*/
	return mesh;
}

string Loader::readGLSL(string filename)
{
	string script = "";

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

	cout << "Loader: Loading GLSL Script: " << scriptFile << endl;

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

		cerr << "Loader: Could not compile " << scriptFile << ": " << endl << errorMessage << endl;

		delete errorMessage;
		return 0;
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

		cerr << "Loader: Could not link shaders: " << errorMessage << endl;

		delete errorMessage;
		return 0;
	}

	return id;
}

unsigned int Loader::loadTexture(std::string file)
{
	SDL_Surface * img = IMG_Load(file.c_str());

	unsigned int result;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, img->format->BytesPerPixel, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);

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

unsigned int Loader::buffer(Mesh * mesh)
{
	unsigned int vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->size(), &mesh->data()[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;


}
