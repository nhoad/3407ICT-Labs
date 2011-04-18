#include <vector>
#include <string>
#include <sstream>

#pragma once
std::string strip(std::string line);

// as shocking as it is to define it here, for some reason it won't compile if I put it in cpp files.
template <typename T> T stringToType(std::string text)
{
	std::istringstream converter(text);

	T result;
	converter >> result;
	return result;
}

template <typename T> std::string typeToString(T thing)
{
	std::ostringstream converter;

	converter << thing;

	return converter.str();
}

std::vector<std::string> tokenize(const std::string text, const std::string pattern);
