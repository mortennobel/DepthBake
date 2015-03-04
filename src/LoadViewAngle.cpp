#include "LoadViewAngle.h"
#include <locale>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// http://stackoverflow.com/a/217605
// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// http://stackoverflow.com/a/217605
// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// http://stackoverflow.com/a/217605
// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}
// http://stackoverflow.com/a/236803
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

// http://stackoverflow.com/a/236803
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::vector<ViewAngle> loadViewAngleFile(std::string filename){
	using namespace std;
	vector<ViewAngle> res;
	ifstream myReadFile;
	
	myReadFile.open(filename);

	if (myReadFile.is_open()) {
		// read header
		std::string line;
		getline(myReadFile, line);
		
		while (!myReadFile.eof()) {
			getline(myReadFile, line);
			if (line.length() < 3) continue;
			auto tokens = split(line, ',');
			std::string fn = trim(tokens[0]);
			double targetAzimuth = atof(tokens[1].c_str());
			double depressionAngle = atof(tokens[2].c_str());
			
			res.push_back(ViewAngle::fromDepressionAngle(fn, depressionAngle, targetAzimuth));
		}
	}
	else {
		cerr << "Could not open file for input " << filename << endl;
		exit(-1);
	}
	myReadFile.close();
	return res;
}

