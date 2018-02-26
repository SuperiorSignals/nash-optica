#pragma once
#include "position.h"

template <class T>
class Ray {
public:
	Position<T> origin;
	Position<T> direction;
	Ray();
	Ray(Position<T> origination, Position<T> heading);
	Ray(const Ray &input);
	~Ray();
	Ray& operator=(const Ray &input);
	void setOrigin(Position<T> input);
	void setDirection(Position<T> intput);
};

template <class T>
Ray<T>::Ray()
{
	direction.setX(1);
	direction.setY(0);
}

template <class T>
Ray<T>::Ray(Position<T> origination, Position<T> heading)
{
	origin = origination;
	direction = heading;
}

template <class T>
Ray<T>::Ray(const Ray<T> &input)
{
	origin = input.origin;
	direction = input.direction;
}

template <class T>
Ray<T>::~Ray()
{

}

template <class T>
Ray<T>& Ray<T>::operator=(const Ray<T> &input)
{
	origin = input.origin;
	direction = input.direction;

	return *this;
}

template <class T>
void Ray<T>::setOrigin(Position<T> input)
{
	origin = input;
}

template <class T>
void Ray<T>::setDirection(Position<T> input)
{
	direction = input;
}