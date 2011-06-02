/**
 * Vec3 implementation
 * Author: Nathan Hoad.
 * Date: 20th May 2011
 */
#include "Vec3.h"
#include "Vec4.h"

#include <cmath>
#include <iostream>
using std::ostream;
using std::endl;

Vec3::Vec3(float * data)
{
	for (int i=0; i < 3; i++)
		this->data[i] = data[i];
}

Vec3::Vec3(float x, float y, float z)
{
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

Vec3 Vec3::normalised()
{
	Vec3 temp = (*this);

	return temp / length();
}

float Vec3::length()
{
	float x = data[0];
	float y = data[1];
	float z = data[2];

	return sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::operator/(const float & l) const
{
	Vec3 temp = (*this);

	for (int i=0; i < 3; i++)
		temp(i) /= l;

	return temp;
}

Vec3 Vec3::operator*(const Vec3 & v) const
{
	Vec3 temp = (*this);

	temp(0) = (*this)(1) * v(2) - (*this)(2) * v(1);
	temp(1) = (*this)(2) * v(0) - (*this)(0) * v(2);
	temp(2) = (*this)(0) * v(1) - (*this)(1) * v(0);

	return temp;
}

Vec3 Vec3::operator-(const Vec3 & v) const
{
	Vec3 temp;

	for (int i=0; i < 3; i++)
		temp(i) = (*this)(i) - v(i);

	return temp;
}

float Vec3::operator()(int x) const
{
	return data[x];
}

float& Vec3::operator()(int x)
{
	return data[x];
}

ostream & operator<<(ostream & o, const Vec3 & v)
{
	for (int i=0; i < 3; i++)
	{
		o.width(10);
		o << v(i);
	}

	return o;
}

Vec3::operator Vec4()
{
	Vec4 r;
	r.x = this->x;
	r.y = this->y;
	r.z = this->z;
	r.w = 1;

	return r;
}
