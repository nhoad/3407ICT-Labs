#include "Vec4.h"

#include <cmath>
#include <iostream>
using std::ostream;
using std::endl;

Vec4::Vec4(float * data)
{
	for (int i=0; i < 4; i++)
		this->data[i] = data[i];
}

Vec4::Vec4(float x, float y, float z, float w)
{
	data[0] = x;
	data[1] = y;
	data[2] = z;
	data[3] = w;
}

Vec4 Vec4::normalised()
{
	Vec4 temp = (*this);

	return temp / length();
}

float Vec4::length()
{
	float x = data[0];
	float y = data[1];
	float z = data[2];
	float w = data[3];

	return sqrt(x*x + y*y + z*z + w*w);
}

Vec4 Vec4::operator/(const float & l) const
{
	Vec4 temp = (*this);

	for (int i=0; i < 4; i++)
		temp(i) /= l;

	return temp;
}

Vec4 Vec4::operator*(const Vec4 & v) const
{
	Vec4 temp = (*this);

	temp(0) = (*this)(1) * v(2) - (*this)(2) * v(1);
	temp(1) = (*this)(2) * v(0) - (*this)(0) * v(2);
	temp(2) = (*this)(0) * v(1) - (*this)(1) * v(0);
	temp(3) = 0;

	return temp;
}

Vec4 Vec4::operator-(const Vec4 & v) const
{
	Vec4 temp;

	for (int i=0; i < 4; i++)
		temp(i) = (*this)(i) - v(i);

	return temp;
}

float Vec4::operator()(int x) const
{
	return data[x];
}

float& Vec4::operator()(int x)
{
	return data[x];
}

ostream & operator<<(ostream & o, const Vec4 & v)
{
	for (int i=0; i < 4; i++)
	{
		o.width(10);
		o << v(i);
	}

	return o;
}

