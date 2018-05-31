#include "sphericallens.h"
#include <cmath>
//#include <GL/glew.h>
#include <qopengl.h>
//#include <iostream>
#include <vector>

#define PI 3.14159265359
SphericalLens::SphericalLens()
{
	front.setRadius(0.1);
	back.setRadius(0.1);
	centerLens();
}

void SphericalLens::changeFrontRadius(GLfloat radius)
{
	front.setRadius(radius);
	centerLens();
}

void SphericalLens::changeBackRadius(GLfloat radius)
{
	back.setRadius(radius);
	centerLens();
}

void SphericalLens::generateIntersection(Position<GLfloat> &top, Position<GLfloat> &bottom)
{
	GLfloat temporary;

	temporary = back.getRadius() * back.getRadius() - front.getRadius() * front.getRadius();
	temporary = temporary - (back.center.getX() * back.center.getX() - front.center.getX() * front.center.getX());
	temporary = temporary / (2 * (back.center.getX() - front.center.getX()));
	if (temporary < 0) {
		temporary = temporary * -1;
	}
	top.setX(temporary);
	bottom.setX(temporary);
	temporary = back.getRadius() * back.getRadius();
	temporary = temporary - (top.getX() - back.center.getX()) * (top.getX() - back.center.getX());
	temporary = sqrt(temporary);
	top.setY(back.center.getY() + temporary);
	bottom.setY(back.center.getY() - temporary);
}

void SphericalLens::generateIntersectionAngle(GLfloat angles[4])
{
	GLfloat temporary;
	Position<GLfloat> top;
	Position<GLfloat> bottom;

	generateIntersection(top, bottom);
	// front start angle
	temporary = (top.getY() - front.center.getY()) / (top.getX() - front.center.getX());
	angles[0] = tanh(temporary);
	if (top.getX() - front.center.getX() < 0) {
		angles[0] = PI - angles[0];
	}
	// front stop angle
	temporary = (bottom.getY() - front.center.getY()) / (bottom.getX() - front.center.getX());
	angles[1] = tanh(temporary);
	if (bottom.getX() - front.center.getX() < 0) {
		angles[1] = PI - angles[1];
	}
	// back start angle
	temporary = (top.getY() - back.center.getY()) / (top.getX() - back.center.getX());
	angles[2] = tanh(temporary);
	if (top.getX() - back.center.getX() < 0) {
		angles[2] = PI - angles[2];
	}
	// back stop angle
	temporary = (bottom.getY() - back.center.getY()) / (bottom.getX() - back.center.getX());
	angles[3] = tanh(temporary);
	if (bottom.getX() - back.center.getX() < 0) {
		angles[3] = PI - angles[3];
	}
}

Sphere<GLfloat> SphericalLens::getBack()
{
	centerLens();
	return back;
}

GLfloat SphericalLens::getBackThickness()
{
	GLfloat output;

	if (hasShaft()) {
		if (isFrontThicker()) {
			return back.getRadius();
		} else {
			output = back.getRadius() - sqrt(back.getRadius() * back.getRadius() - front.getRadius() * front.getRadius());
			return output;
		}
	} else {
		output = 2 * front.getRadius() * getThickness() - getThickness() * getThickness();
		output = output / (front.getRadius() + back.getRadius() - getThickness());
		output = 0.5 * output;
		return output;
	}
}

GLfloat SphericalLens::getFocalLength()
{
	GLfloat focalLength;
	GLfloat temporary;

	temporary = 1 / front.getRadius() - 1 / back.getRadius();
	temporary *= refractiveIndex - 1;
	focalLength = 1 / temporary;

	return focalLength;
}

Sphere<GLfloat> SphericalLens::getFront()
{
	centerLens();
	return front;
}

GLfloat SphericalLens::getFrontThickness()
{
	GLfloat output;

	if (hasShaft()) {
		if (isFrontThicker()) {
			output = front.getRadius() - sqrt(front.getRadius() * front.getRadius() - back.getRadius() * back.getRadius());
			return output;
		} else {
			return front.getRadius();
		}
	} else {
		output = 2 * back.getRadius() * getThickness() - getThickness() * getThickness();
		output = output / (front.getRadius() + back.getRadius() - getThickness());
		output = 0.5 * output;
		return output;
	}
}

GLfloat SphericalLens::getLensHeight()
{
	GLfloat output;
	GLfloat temporary;

	temporary = getFrontThickness();
	output = 2 * front.getRadius() * temporary - temporary * temporary;
	output = sqrt(output);

	return output;
}

void SphericalLens::setThickness(GLfloat input)
{
	Lens::setThickness(input);
	centerLens();
}

void SphericalLens::centerLens()
{
	Position<GLfloat> backCenter, frontCenter;

	frontCenter.setX(0 + front.getRadius() - getFrontThickness());
	frontCenter.setY(0);
	backCenter.setX(0 - back.getRadius() + getBackThickness());
	frontCenter.setY(0);
	front.setCenter(frontCenter);
	back.setCenter(backCenter);
}

GLfloat SphericalLens::getFrontIntersectionAngle()
{
	GLfloat output;

	output = getLensHeight() / front.getRadius();
	output = asin(output);

	return output;
}

GLfloat SphericalLens::getBackIntersectionAngle()
{
	GLfloat output;

	output = getLensHeight() / back.getRadius();
	output = asin(output);

	return output;
}

bool SphericalLens::hasShaft()
{
	if (isFrontThicker()) {
		if (getThickness() > back.getRadius()) {
			return true;
		} else {
			return false;
		}
	} else {
		if (getThickness() > front.getRadius()) {
			return true;
		} else {
			return false;
		}
	}
}

bool SphericalLens::isFrontThicker()
{
	if (front.getRadius() > back.getRadius()) {
		return true;
	}

	return false;
}

std::vector<Position<GLfloat>> lensProfile(SphericalLens input, int numberOfPoints)
{
	std::vector<Position<GLfloat>> output;
	std::vector<Position<GLfloat>> temporary;
	int length;

    //std::cout << "Front intersection angle: " << input.getFrontIntersectionAngle() << std::endl;
    //std::cout << "Back intersection angle: " << input.getBackIntersectionAngle() << std::endl;
	output = sphereArc(input.getFront(), - PI, - PI + input.getFrontIntersectionAngle(), numberOfPoints / 2);
	temporary = sphereArc(input.getBack(), input.getBackIntersectionAngle(), 0, numberOfPoints / 2);
	for (int i = 0; i < temporary.size(); i++) {
		output.push_back(temporary[i]);
	}

	return output;
}
