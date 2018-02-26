#pragma once

template <class T>
class Position {
public:
	Position();
	Position(T abscissa, T ordinate);
	Position(const Position &input);
	~Position(void);
	Position& operator=(const Position &input);
	T getX(void);
	T getY(void);
	void setX(T abscissa);
	void setY(T abscissa);
	void setPosition(T abscissa, T ordinate);
private:
	T x;
	T y;
};

template <class T>
Position<T>::Position(void)
{
	x = 0;
	y = 0;
}

template <class T>
Position<T>::Position(T abscissa, T ordinate)
{
	x = abscissa;
	y = ordinate;
}

template <class T>
Position<T>::Position(const Position &input)
{
	x = input.x;
	y = input.y;
}

template <class T>
Position<T>::~Position(void)
{
	
}

template <class T>
Position<T>& Position<T>::operator=(const Position &input)
{
	x = input.x;
	y = input.y;

	return *this;
}

template <class T>
T Position<T>::getX(void) 
{
	return x;
}

template <class T>
T Position<T>::getY(void) 
{
	return y;
}

template <class T>
void Position<T>::setX(T abscissa)
{
	x = abscissa;
}

template <class T>
void Position<T>::setY(T ordinate)
{
	y = ordinate;
}

template <class T>
void Position<T>::setPosition(T abscissa, T ordinate)
{
	x = abscissa;
	y = ordinate;
}