/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#include "Primitives.h"
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
		Mesh static readMesh(const std::string filename);

		/**
			Read a GLSL script in from a file.

			\param filename the path to the file containing the object
			\return the script from input file as a string string.
		*/
		std::string static readGLSL(const std::string filename);

		/**
			Create a shader from a script.

			\param filepath script the script to load in
			\param shaderType either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
		*/
		unsigned int static loadShader(std::string script, int shaderType);

		/**
		 * Link a vertex and a fragment shader.
		 * \param vertex the id of the vertex shader returned by loadShader()
		 * \param fragment id of the fragment shader returns by laodShader()
		 * \return program id of the resulting shader program
		 */
		unsigned int static linkShader(unsigned int vertex, unsigned int fragment);

		unsigned int static loadTexture(std::string file);
};
