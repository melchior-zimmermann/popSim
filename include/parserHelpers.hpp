#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

using std::cout;
using std::endl;
using std::ifstream;
using std::invalid_argument;
using std::out_of_range;
using std::stod;
using std::stoul;
using std::string;
using std::vector;

/**
 * @file parserHelpers.hpp
 * @author Melchior Zimmermann
 * @date 28 Mar 2017
 * @brief File containing helper functions used for parsing parameter files.
 *
 * The methods in this file are used to parse paramter values into a simParams struct from a
 * parameter file.
 */

vector<string> split(const string& line, char delimiter);
string prune(const string& line, char delimiter);
bool findName(ifstream& file, const string name, string& line);
bool readValue(const string& line, double& d);
bool readValue(const string& line, vector<double>& d);
bool readValue(const string& line, int& d);
bool readValue(const string& line, vector<int>& d);
bool readValue(const string& line, char& d);
bool readValue(const string& line, bool& b);
inline void rewind(ifstream& file) {
		file.clear();
		file.seekg(0);
	}