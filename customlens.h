#pragma once
#ifndef CUSTOMLENS_H_
#define CUSTOMLENS_H_

#include "lens.h"
#include <cmath>
#include <vector>

class CustomLens : public Lens {
public:
	std::vector<double> getAngles();
	std::vector<double> getLensAngles();
	GLfloat getFocalLength();
	GLfloat getBaseThickness();
	void setAngles(std::vector<double> input);
	void setLensAngles(std::vector<double> input);
	void setFocalLength(GLfloat value);
	void setBaseThickness(GLfloat value);
private:
	GLfloat focalLength;
	GLfloat baseThickness;
	std::vector<double> angles;
	std::vector<double> lensAngles;
};
#endif // !CUSTOMLENS_H_