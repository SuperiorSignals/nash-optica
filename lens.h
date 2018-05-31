#pragma once
#ifndef LENS_H_
#define LENS_H_

//#include <gl/glew.h>
#include <qopengl.h>

class Lens {
public:
	Lens();
	GLfloat getRefractiveIndex();
	GLfloat getThickness();
	void setRefractiveIndex(GLfloat index);
	void setThickness(GLfloat input);

	virtual GLfloat getFocalLength();
private:
	GLfloat refractiveIndex;
	GLfloat thickness;
};

#endif // !LENS_H_
