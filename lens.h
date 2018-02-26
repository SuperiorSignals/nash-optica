#pragma once
#include "sphere.h"
#include <cmath>

#define PI 3.14159265359

class Lens {
public:
	Lens();
	Sphere<GLfloat> front;
	Sphere<GLfloat> back;
	GLfloat getRefractiveIndex(void);
	void setRefractiveIndex(GLfloat index);
	void generateIntersection(Position<GLfloat> &top, Position<GLfloat> &bottom);
	void generateIntersectionAngle(GLfloat angles[4]);
private:
	GLfloat refractiveIndex;
};

Lens::Lens()
{
	front.center.setX(0.5);
	front.center.setY(0.5);
	front.setRadius(0.1);

	back.center.setX(0.31);
	back.center.setY(0.5);
	back.setRadius(0.1);

	refractiveIndex = 1.0;
}

GLfloat Lens::getRefractiveIndex(void)
{
	return refractiveIndex;
}

void Lens::setRefractiveIndex(GLfloat index)
{
	refractiveIndex = index;
}

void Lens::generateIntersection(Position<GLfloat> &top, Position<GLfloat> &bottom)
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

void Lens::generateIntersectionAngle(GLfloat angles[4])
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

