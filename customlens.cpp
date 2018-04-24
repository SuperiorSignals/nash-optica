#include "customlens.h"
#include <vector>

std::vector<double> CustomLens::getAngles()
{
	return angles;
}

std::vector<double> CustomLens::getLensAngles()
{
	return lensAngles;
}

GLfloat CustomLens::getFocalLength()
{
	return focalLength;
}

GLfloat CustomLens::getBaseThickness()
{
	return baseThickness;
}

void CustomLens::setAngles(std::vector<double> input)
{
	angles = input;
}

void CustomLens::setLensAngles(std::vector<double> input)
{
	lensAngles = input;
}

void CustomLens::setFocalLength(GLfloat value)
{
	focalLength = value;
}

void CustomLens::setBaseThickness(GLfloat value)
{
	baseThickness = value;
}