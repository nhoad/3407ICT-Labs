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
	vector<string> results;

	string str = text;

	int cutAt;
	while( (cutAt = str.find_first_of(pattern)) != string::npos )
	{
		if(cutAt > 0)
			results.push_back(str.substr(0, cutAt));

		str = str.substr(cutAt+1);
	}
	if(str.length() > 0)
		results.push_back(str);

	return results;
}
