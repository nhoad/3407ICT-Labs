/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 */
#include "Transformation.h"

#define PI 3.14159265
#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

#include <cmath>
#include <cstdlib>

Mat4::Mat4()
{
	for (int i=0; i < 16; i++)
		data[i] = (i % 5 == 0) ? 1 : 0;
}

Mat4::Mat4(float defaultValue)
{
	for (int i=0; i < 16; i++)
		data[i] = defaultValue;
}

Mat4::Mat4(float * values)
{
	for (int i=0; i < 16; i++)
		data[i] = values[i];
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

Mat4::operator float*()
{
	return data;
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

float Mat4::operator()(int x) const
{
	return data[x];
}

float& Mat4::operator()(int x)
{
	return data[x];
}


float Mat4::operator()(int x, int y) const
{
	return data[(x * 4) + y];
}

float& Mat4::operator()(int x, int y)
{
	return data[(x * 4) + y];
}

Mat4& Mat4::operator=(const Mat4& m)
{
	for (int i=0; i < 16; i++)
		(*this)(i) = m(i);

	return *this;

}

Mat4& Mat4::operator*=(Mat4 m)
{
	*this = *this * m;
	return *this;
}

Mat4 Mat4::operator*(const Mat4& m)
{
	return Mat4::mul(*this, m);
}

Mat4 Mat4::mul(const Mat4& m, const Mat4& n)
{
	Mat4 r(0.0);
	for (int row=0; row < 4; row++)
		for (int column=0; column < 4; column++)
			for (int i=0; i < 4; i++)
				r(row, column) += m(row, i) * n(i, column);
	return r;
}

Vec4 Mat4::mul(const Mat4& m, const Vec4& v)
{
	Vec4 r(0, 0, 0, 1);

	for (int i=0; i < 4; i++)
		for (int j=0; j < 4; j++)
			r(i) += m(j, i) * v(j);

	return r;
}

Mat4 Mat4::translate(float x, float y, float z)
{
	Mat4 r;

	r(3, 0) = x;
	r(3, 1) = y;
	r(3, 2) = z;

	return r;
}

Mat4 Mat4::rotateX(float degree)
{
	Mat4 r;

	degree = (degree * PI) / 180;

	const float c = cos(degree);
	const float s = sin(degree);

	r(1, 1) = c;  r(1, 2) = s;
	r(2, 1) = -s; r(2, 2) = c;

	return r;
}

Mat4 Mat4::rotateY(float degree)
{
	Mat4 r;

	degree = (degree * PI) / 180;

	const float c = cos(degree);
	const float s = sin(degree);

	r(0, 0) = c; r(0, 2) = -s;
	r(2, 0) = s; r(2, 2) = c;

	return r;
}

Mat4 Mat4::rotateZ(float degree)
{
	Mat4 r;

	degree = (degree * PI) / 180;

	const float c = cos(degree);
	const float s = sin(degree);

	r(0, 0) = c;  r(0, 1) = -s;
	r(1, 0) = s; r(1, 1) = c;

	return r;
}

Mat4 Mat4::scale(float x, float y, float z)
{
	Mat4 r;

	r(0, 0) = x;
	r(1, 1) = y;
	r(2, 2) = z;

	return r;
}

Mat4 Mat4::perspectiveMatrix(float fieldOfView, float aspectRatio, float n, float f)
{
	Mat4 m(0.0);

	const float t = n * tan(fieldOfView * (PI / 360.0));
	const float b = -t;
	const float r = t * aspectRatio;
	const float l = -r;

	m(0,0) = (2.0 * n) / (r - l);
	m(1,1) = (2.0 * n) / (t - b);
	m(2,2) = -(f + n) / (f - n);
	m(2,3) = -1.0;
	m(3,2) = (-2.0 * f * n) / (f - n);

	return m;
}

Mat4 Mat4::lookAt(Vec4 & camera, Vec4 & target, Vec4 & up)
{
	Mat4 trans = Mat4::translate(-camera(0), -camera(1), -camera(2));
	Mat4 rot;

	Vec4 forward = (target - camera).normalised();
	Vec4 left = (up * forward).normalised();
	Vec4 upAxis = forward * left;

	rot(0, 0) = left(0); rot(0, 1) = upAxis(0); rot(0, 2) = forward(0);
	rot(1, 0) = left(1); rot(1, 1) = upAxis(1); rot(1, 2) = forward(1);
	rot(2, 0) = left(2); rot(2, 1) = upAxis(2); rot(2, 2) = forward(2);

	return rot * trans;
}

ostream & operator<<(ostream & o, const Mat4 & m)
{
	for (int i=0; i < 16; i++)
	{
		if (i % 4 == 0)
			o << endl;

		o.width(10);
		o << m(i);

	}

	return o;
}

#if TEST
int main(void)
{
	Mat4 t =  Mat4::translate(2, 3, 4);

	Vec4 camera(0,0,1);
	Vec4 target(0,0,0);
	Vec4 up(0,-1, 0);

	cout << Mat4::lookAt(camera, target, up) << endl;

	cout << t << endl;

	cout << t(12) << endl;
	cout << t(13) << endl;
	cout << t(14) << endl;
}
#endif
