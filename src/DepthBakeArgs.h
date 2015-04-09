#pragma once
#include <string>

class DepthBakeArgs {
	
	char ** begin;
	char ** end;

	char* getCmdOption(const std::string & option);

	bool cmdOptionExists(const std::string& option);

	std::string getCmdOptionString(const std::string & option, std::string defaultValue);

	float getCmdOptionFloat(const std::string & option, float defaultValue);

	int getCmdOptionInt(const std::string & option, int defaultValue);

	void printHelp();
public:
	float elevationMinAngle = 15;
	float elevationMaxAngle = 45;
	float elevationStepSize = 5;
	int azimuthSteps = 36;
	int resolution = 512; // number of pixels in width / height of image plane
	float cameraResolution = 20; // size of image plane in width / height (in meters)
	float cameraRadius = 100;
	bool flipYZ = false;
    bool centerUsingBoundaryBox = false;
	std::string inputUnit = "inch";
	std::string viewAngleFile = "";
    std::string outputPath = "";

	DepthBakeArgs(int argc, char * argv[]);

	std::string getModelFilename();

};