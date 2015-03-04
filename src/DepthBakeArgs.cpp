#include "DepthBakeArgs.h"

#include <algorithm>
#include <iostream>

using namespace std;

// http://stackoverflow.com/a/868894
char* DepthBakeArgs::getCmdOption(const std::string & option)
{
	char ** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool DepthBakeArgs::cmdOptionExists(const std::string& option)
{
	return std::find(begin, end, option) != end;
}

std::string DepthBakeArgs::getCmdOptionString(const std::string & option, std::string defaultValue){
	char * res = getCmdOption( option);
	if (res){
		return res;
	}
	return defaultValue;
}

float DepthBakeArgs::getCmdOptionFloat(const std::string & option, float defaultValue){
	char * res = getCmdOption(option);
	if (res){
		return (float)atof(res);
	}
	return defaultValue;
}

int DepthBakeArgs::getCmdOptionInt(const std::string & option, int defaultValue){
	char * res = getCmdOption( option);
	if (res){
		return atoi(res);
	}
	return defaultValue;
}

void DepthBakeArgs::printHelp(){
	cout << "Usage:\n" <<
		"DepthBake [args] [model-file]" <<
		"\n-elevationMinAngle [float]" << elevationMinAngle<<
		"\n-elevationMaxAngle [float]" << elevationMaxAngle<<
		"\n-elevationStepSize [float]" << elevationStepSize<<
		"\n-azimuthSteps [int]" << azimuthSteps <<
		"\n-resolution [int]" << resolution <<
		"\n-cameraResolution [float]" << cameraResolution <<
		"\n-cameraRadius [int]" << cameraRadius <<
		"\n-flipYZ " <<
		"\n-inputUnit [inch,mm]" << inputUnit<<
		"\n-viewAngleFile [] "<<
		"\n";
}

string DepthBakeArgs::getModelFilename(){
	return std::string(end[-1]);
}

DepthBakeArgs::DepthBakeArgs(int argc, char * argv[]){
	begin = argv;
	end = argv + argc;
	if (cmdOptionExists("-?") ||
		cmdOptionExists("--?") ||
		cmdOptionExists("/?") ||
		cmdOptionExists("--help") ||
		cmdOptionExists("-help") ||
		cmdOptionExists("/help")){
		printHelp();
		exit(0);
	}
	elevationMinAngle = getCmdOptionFloat("-elevationMinAngle", elevationMinAngle);
	elevationMaxAngle = getCmdOptionFloat("-elevationMaxAngle", elevationMaxAngle);
	elevationStepSize = getCmdOptionFloat("-elevationStepSize", elevationStepSize);
	azimuthSteps = getCmdOptionInt("-azimuthSteps", azimuthSteps);
	resolution = getCmdOptionInt("-resolution", resolution);
	cameraResolution = getCmdOptionFloat("-cameraResolution", cameraResolution);
	cameraRadius = getCmdOptionFloat("-cameraRadius", cameraRadius);
	flipYZ = cmdOptionExists("-flipYZ");
	inputUnit = getCmdOptionString("-inputUnit", inputUnit);
	if (inputUnit != "inch" && inputUnit != "mm"){
		std::cerr << "Unsupported input unit. mm or inch expected" << endl;
		exit(0);
	}
	viewAngleFile = getCmdOptionString("-viewAngleFile", viewAngleFile);
}