#include "Vec2.h"

#include <cmath>
#include <iostream>
using std::ostream;
using std::endl;

Vec2::Vec2(float * data)
{
	for (int i=0; i < 2; i++)
		this->data[i] = data[i];
}

Vec2::Vec2(float x, float y)
{
	data[0] = x;
	data[1] = y;
}

Vec2 Vec2::normalised()
{
	Vec2 temp = (*this);

	return temp / length();
}

float Vec2::length()
{
	float x = data[0];
	float y = data[1];

	return sqrt(x*x + y*y);
}

Vec2 Vec2::operator/(const float & l) const
{
	Vec2 temp = (*this);

	for (int i=0; i < 2; i++)
		temp(i) /= l;

	return temp;
}

Vec2 Vec2::operator*(const Vec2 & v) const
{
	Vec2 temp = (*this);

	temp(0) = (*this)(1) * v(2) - (*this)(2) * v(1);
	temp(1) = (*this)(2) * v(0) - (*this)(0) * v(2);

	return temp;
}

Vec2 Vec2::operator-(const Vec2 & v) const
{
	Vec2 temp;

	for (int i=0; i < 2; i++)
		temp(i) = (*this)(i) - v(i);

	return temp;
}

float Vec2::operator()(int x) const
{
	return data[x];
}

float& Vec2::operator()(int x)
{
	return data[x];
}

ostream & operator<<(ostream & o, const Vec2 & v)
{
	for (int i=0; i < 2; i++)
	{
		o.width(10);
		o << v(i);
	}

	return o;
}


