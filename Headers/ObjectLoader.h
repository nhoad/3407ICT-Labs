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
		*/
		void read(const std::string filename);

		/**
			Returns the object as it was read in from the file.

			\return the object mesh.
		*/
		Mesh object();

	private:
		Mesh mesh;
};
