#include "specification.h"
#include <vector>

std::vector<double> Specification::getAngles()
{
	return angles;
}

std::vector<double> Specification::getGivenIntensity()
{
	return givenIntensity;
}

std::vector<double> Specification::getDesiredIntensity()
{
	return desiredIntensity;
}

void Specification::setAngles(std::vector<double> input)
{
	angles = input;
}

void Specification::setGivenIntensity(std::vector<double> input)
{
	givenIntensity = input;
}

void Specification::setDesiredIntensity(std::vector<double> input)
{
	desiredIntensity = input;
}