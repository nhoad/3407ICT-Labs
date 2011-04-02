#include <vector>
#include <string>
#include <sstream>

std::string strip(std::string line);

// as shocking as it is to define it here, for some reason it won't compile if I put it in cpp files.
template <typename T> T stringToType(string text)
{
	std::istringstream converter(text);

	T result;
	converter >> result;
	return result;
}


std::vector<std::string> tokenize(const std::string text, const std::string pattern);
