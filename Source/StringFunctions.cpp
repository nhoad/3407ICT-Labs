/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */

#include <string>
using std::string;

#include <sstream>
using std::istringstream;

#include "StringFunctions.h"
using std::vector;

string strip(string line)
{
	size_t trim_pos = line.find_first_not_of(" \n\t");

	if (trim_pos == string::npos)
		return line;

	line = line.substr(trim_pos, line.length() - trim_pos + 1);
	trim_pos = line.find_last_not_of(" \n\t");

	if (trim_pos == string::npos)
		return line;
	line = line.substr(0, trim_pos + 1);

	return line;
}

vector<string> tokenize(const string text, const string pattern)
{
	vector<string> result;

	size_t lastPos = text.find_first_not_of(pattern, 0);
	size_t firstPos = text.find_first_of(pattern, lastPos);

	while (firstPos != string::npos)
	{
		result.push_back(text.substr(lastPos, firstPos - lastPos));
		lastPos = text.find_first_not_of(pattern, firstPos);
		firstPos = text.find_first_of(pattern, lastPos);
	}

	result.push_back(text.substr(lastPos, firstPos - lastPos));

	return result;
}
