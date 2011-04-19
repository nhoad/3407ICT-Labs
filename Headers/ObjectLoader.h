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
		*/
		void read(const std::string filename, bool rgbMagic);

		/**
			Returns the object as it was read in from the file.

			\return the object mesh.
		*/
		Mesh object();

	private:
		Mesh mesh;
};
