#pragma once
#include <string>

class ViewAngle{
	std::string filename;
	double elevation; // rotation around z axis
	double azimuth; // rotation around up-axis
public:
	ViewAngle(){}

	ViewAngle(std::string filename, double elevation, double azimuth) :filename(filename), elevation(elevation), azimuth(azimuth){}
	
	static ViewAngle fromDepressionAngle(std::string filename, double depressionAngle, double azimuth){
		return ViewAngle(filename, depressionAngle, azimuth);
	}
	
	std::string getFilename(){ return filename; }
	double getElevation(){ 
		//return elevation; 
		return 70;
	}
	double getAzimuth(){ 
		//return azimuth;
		return 90;
	}
};