#include "Primitives.h"
#include <string>

class ObjectLoader
{
	public:
		void read(const std::string filename);
		Mesh object();

	private:
		Mesh mesh;
};
