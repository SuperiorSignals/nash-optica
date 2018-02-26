#pragma once
#include "position.h"

template <class T>
class Sphere {
public:
	Position<T> center;

	Sphere();
	Sphere(Position<T> theCenter, T theRadius);
	T getRadius(void);
	void setRadius(T theRadius);
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
T Sphere<T>::getRadius(void)
{
	return radius;
}

template <class T>
void Sphere<T>::setRadius(T theRadius)
{
	radius = theRadius;
}