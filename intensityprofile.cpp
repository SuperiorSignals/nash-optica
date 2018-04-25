#include "intensityprofile.h"
#include <vector>

IntensityProfile::IntensityProfile()
{

}

std::vector<double> IntensityProfile:: getAngles()
{
	return angles;
}

std::vector<double> IntensityProfile::getIntensity()
{
	return intensity;
}

double IntensityProfile::getIntensityAtAngle(double angle)
{
	int closestAngle;
	double minimumDistance;
	double output;

	if (angles.size() > 0) {
		minimumDistance = abs(angle - angles[0]);
		closestAngle = 0;
	}
	for (int i = 0; i < angles.size(); i++) {
		if (abs(angle - angles[i]) < minimumDistance) {
			minimumDistance = abs(angle - angles[i]);
			closestAngle = i;
		}
	}
	output = intensity[closestAngle]; // Default set to the closest matching value
	if (angles.size() - 1 > closestAngle) {
		//output = (angle - angles[closestAngle]) * (intensity[closestAngle + 1] - intensity[closestAngle]) / (angles[closestAngle + 1] - angles[closestAngle]) + intensity[closestAngle];
		output = (angle - angles[closestAngle]);
		output *= (intensity[closestAngle + 1] - intensity[closestAngle]) / (angles[closestAngle + 1] - angles[closestAngle]);
		output += intensity[closestAngle];
	} else if(closestAngle > 0) {
		//output = (angle - angles[closestAngle]) * (intensity[closestAngle] - intensity[closestAngle - 1]) / (angles[closestAngle] - angles[closestAngle - 1]) + intensity[closestAngle];
		output = (angle - angles[closestAngle]);
		output *= (intensity[closestAngle] - intensity[closestAngle - 1]) / (angles[closestAngle] - angles[closestAngle - 1]);
		output += intensity[closestAngle];
	}
	
	return output;
}

void IntensityProfile::setAngles(std::vector<double> input)
{
	angles = input;
}

void IntensityProfile::setIntensity(std::vector<double> input)
{
	intensity = input;
}