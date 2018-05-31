#include "sphere.h"
#include <cmath>
//#include <GL/glew.h>
#include <qopengl.h>
#include <vector>
#include "position.h"

std::vector<Position<GLfloat>> sphereArc(Sphere<GLfloat> input, GLfloat angle, int numberOfPoints)
{
	std::vector<Position<GLfloat>> output;
	Position<GLfloat> temporary;
	GLfloat angleIncrement;
	GLfloat currentAngle;
	GLfloat position;
	GLfloat radius;

	currentAngle = 0;
	angleIncrement = angle / static_cast<GLfloat>(numberOfPoints);
	radius = input.getRadius();
	for (int i = 0; i < numberOfPoints; i++) {
		position = radius * cos(currentAngle);
		temporary.setX(position + (input.getCenter()).getX());
		position = radius * sin(currentAngle);
		temporary.setY(position + (input.getCenter()).getY());
		output.push_back(temporary);
		currentAngle += angleIncrement;
	}

	return output;
}

std::vector<Position<GLfloat>> sphereArc(Sphere<GLfloat> input, GLfloat startAngle, GLfloat stopAngle, int numberOfPoints)
{
	std::vector<Position<GLfloat>> output;
	Position<GLfloat> temporary;
	GLfloat angleIncrement;
	GLfloat currentAngle;
	GLfloat position;
	GLfloat radius;

	currentAngle = startAngle;
	angleIncrement = (stopAngle - startAngle) / static_cast<GLfloat>(numberOfPoints);
	radius = input.getRadius();
	for (int i = 0; i < numberOfPoints; i++) {
		position = radius * cos(currentAngle);
		temporary.setX(position + (input.getCenter()).getX());
		position = radius * sin(currentAngle);
		temporary.setY(position + (input.getCenter()).getY());
		output.push_back(temporary);
		currentAngle += angleIncrement;
	}

	return output;
}
