#pragma once
#ifndef INTENSITYPROFILE_H_
#define INTENSITYPROFILE_H_
#include <vector>


class IntensityProfile {
public:
	IntensityProfile();
	std::vector<double> getAngles();
	std::vector<double> getIntensity();
	double getIntensityAtAngle(double angle);
	void setAngles(std::vector<double> input);
	void setIntensity(std::vector<double> input);
private:
	std::vector<double> angles;
	std::vector<double> intensity;
};

#endif // !INTENSITYPROFILE_H_

