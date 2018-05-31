#pragma once
#ifndef SPHERE_H_
#define SPHERE_H_

#include "position.h"
//#include <GL/glew.h>
#include <qopengl.h>
#include <vector>

template <class T>
class Sphere {
public:
	Position<T> center;

	Sphere();
	Sphere(Position<T> theCenter, T theRadius);
	Position<T> getCenter();
	T getRadius();
	void setCenter(Position<T> center);
	void setRadius(T theRadius);

	friend std::vector<Position<GLfloat>> sphereArc(Sphere<GLfloat> input, GLfloat angle, int numberOfPoints);
	friend std::vector<Position<GLfloat>> sphereArc(Sphere<GLfloat> input, GLfloat startAngle, GLfloat stopAngle, int numberOfPoints);

private:
	T radius;
};

template <class T>
Sphere<T>::Sphere()
{
	radius = 1;
}

template <class T>
Sphere<T>::Sphere(Position<T> theCenter, T theRadius)
{
	center = theCenter;
	radius = theRadius;
}

template <class T>
Position<T> Sphere<T>::getCenter()
{
	return center;
}

template <class T>
T Sphere<T>::getRadius()
{
	return radius;
}

template <class T>
void Sphere<T>::setCenter(Position<T> input)
{
	center = input;
}

template <class T>
void Sphere<T>::setRadius(T theRadius)
{
	radius = theRadius;
}

#endif // !SPHERE_H_
