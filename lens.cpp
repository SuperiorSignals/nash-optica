#include "lens.h"

Lens::Lens()
{
	refractiveIndex = 1.0;
	thickness = 0.01;
}

GLfloat Lens::getRefractiveIndex()
{
	return refractiveIndex;
}

GLfloat Lens::getThickness()
{
	return thickness;
}

void Lens::setRefractiveIndex(GLfloat index)
{
	refractiveIndex = index;
}

void Lens::setThickness(GLfloat input)
{
	thickness = input;
}

GLfloat
Lens::getFocalLength()
{
	return 0;
}