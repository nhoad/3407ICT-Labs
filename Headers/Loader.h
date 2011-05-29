/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#include "Primitives.h"
#include "Terrain.h"
#include <string>

#pragma once

/**
	Class used to load an object from an obj file.
*/
class Loader
{
	public:
		/**
			Read an object in from a file.

			\param filename the path to the file containing the object
			\return the mesh from input file.
		*/
		static Mesh * readMesh(const std::string filename);

		/**
			Read a GLSL script in from a file.

			\param filename the path to the file containing the object
			\return the script from input file as a string string.
		*/
		static std::string readGLSL(const std::string filename);

		/**
			Create a shader from a script.

			\param filepath script the script to load in
			\param shaderType either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
		*/
		static unsigned int loadShader(std::string script, int shaderType);

		/**
		 * Link a vertex and a fragment shader.
		 * \param vertex the id of the vertex shader returned by loadShader()
		 * \param fragment id of the fragment shader returns by laodShader()
		 * \return program id of the resulting shader program
		 */
		static unsigned int linkShader(unsigned int vertex, unsigned int fragment);

		static unsigned int loadTexture(std::string file);

		static Terrain * loadTerrain(std::string heightmap, float divisions);

		static Vec3 getPixel(SDL_Surface * img, int x, int y);

		static unsigned int buffer(Mesh * mesh);
};
