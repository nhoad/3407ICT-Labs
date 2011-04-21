#include "Primitives.h"
#include <string>

#pragma once

/**
	Class used to load an object from an obj file.
*/
class ObjectLoader
{
	public:
		/**
			Read an object in from a file.

			\param filename the path to the file containing the object
			\param rgbMagic true means use the coords as colors, false will be green.
			\return the mesh from input file.
		*/
		Mesh read(const std::string filename, bool rgbMagic);

};
