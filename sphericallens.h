#pragma once
#include "lens.h"
#include <GL/glew.h>
#include <vector>
#include "position.h"
#include "sphere.h"

//std::vector<Position<GLfloat>> lensProfile(GLfloat angle, int numberOfPoints);

class SphericalLens: public Lens {
public:
	SphericalLens();
	void changeFrontRadius(GLfloat radius);
	void changeBackRadius(GLfloat radius);
	void generateIntersection(Position<GLfloat> &top, Position<GLfloat> &bottom);
	void generateIntersectionAngle(GLfloat angles[4]);
	Sphere<GLfloat> getBack();
	GLfloat getBackThickness();
	GLfloat getFocalLength();
	Sphere<GLfloat> getFront();
	GLfloat getFrontThickness();
	GLfloat getLensHeight();
	void setThickness(GLfloat input);

	friend std::vector<Position<GLfloat>> lensProfile(SphericalLens input, int numberOfPoints);

private:
	Sphere<GLfloat> front;
	Sphere<GLfloat> back;
	void centerLens();
	GLfloat getBackIntersectionAngle();
	GLfloat getFrontIntersectionAngle();
	bool hasShaft();
	bool isFrontThicker();
	GLfloat refractiveIndex;
};


