#include <vector>
#include <string>
#include <sstream>

#pragma once

/**
	Strip all the whitespace from the left and right of a string.

	\param line the line to strip.
	\return line with no leading or following whitespace.
*/
std::string strip(std::string line);

/**
	Convert a String to any type supported by stream operators.

	\param text the text you want converted.
	\return object/primitive of type T equivalent to string.
*/
template <typename T> T stringToType(std::string text)
{
	std::istringstream converter(text);

	T result;
	converter >> result;
	return result;
}

/**
	Convert any type to a String supporting the << operators.

	\param thing the object/primitive you want converted.
	\return string formatted version of thing.
*/
template <typename T> std::string typeToString(T thing)
{
	std::ostringstream converter;

	converter << thing;

	return converter.str();
}

/**
	String tokenizer function.

	\param text the text to tokenize.
	\param pattern the pattern to tokenize with.
	\return the tokenized string.
*/
std::vector<std::string> tokenize(const std::string text, const std::string pattern);
