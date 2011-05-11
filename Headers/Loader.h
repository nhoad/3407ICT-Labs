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
		Mesh readObject(const std::string filename);

		/**
			Read a GLSL script in from a file.

			\param filename the path to the file containing the object
			\return the script from input file as a string string.
		*/
		std::string readGLSL(const std::string filename);

};
