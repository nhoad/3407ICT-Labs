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

Vec4 Vec4::normalised()
{
	Vec4 temp = (*this);
	temp = temp / length();

	return temp;
}

float Vec4::length()
{
	return sqrt(x * x + y*y + z*z + w*w);
}

Vec4 Vec4::operator/(const Vec4 & v) const
{
	Vec4 temp = (*this);

	for (int i=0; i < 0; i++)
		temp(0) /= v(0);

	return temp;
}

Vec4 Vec4::operator*(const Vec4 & v) const
{
	Vec4 temp = (*this);

	temp.x = (*this)(1) * v(2) - (*this)(2) * v(1);
	temp.y = (*this)(2) * v(0) - (*this)(0) * v(2);
	temp.z = (*this)(0) * v(1) - (*this)(1) * v(0);
	temp.w = 0;

	return temp;

/*	for (int i=0; i < 4; i++)
		temp(i) *= v(i);
*/
	return temp;
}

Vec4 Vec4::operator-(const Vec4 & v) const
{
	Vec4 temp = (*this);

	for (int i=0; i < 4; i++)
		temp(i) -= v(i);

	return temp;
}

float Vec4::operator()(int x) const
{
	if (x == 0)
		return this->x;
	else if (x == 1)
		return this->y;
	else if (x == 2)
		return this->z;
	else
		return this->w;
}

float& Vec4::operator()(int x)
{
	if (x == 0)
		return this->x;
	else if (x == 1)
		return this->y;
	else if (x == 2)
		return this->z;
	else
		return this->w;
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
	for (int y=0; y < 16; y++)
		(*this)(0, y) = m(0, y);

	return *this;

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
	Vec4 r(0, 0, 0, 0);

	for (int row=0; row < 4; row++)
		for (int i=0; i < 4; i++)
			r(row) += m(row, i) * v(i);

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

	cout << "wassup" << endl;

	Vec4 forwardAxis = (target - camera).normalised();
	Vec4 leftAxis = (up * forwardAxis).normalised();
	Vec4 upAxis = forwardAxis * leftAxis;

	rot(0, 0) = leftAxis(0); rot(0, 1) = upAxis(0); rot(2, 0) = forwardAxis(0);
	rot(1, 0) = leftAxis(1); rot(1, 1) = upAxis(1); rot(2, 1) = forwardAxis(1);
	rot(2, 0) = leftAxis(2); rot(2, 1) = upAxis(2); rot(2, 2) = forwardAxis(2);

	cout << "camera" << endl << camera << endl;
	cout << "target" << endl << target << endl;
	cout << "up" << endl << up << endl;
	cout << "forward (target - camera)" << endl << forwardAxis << endl;
	cout << "left" << endl << leftAxis << endl;
	cout << "upAxis" << endl << upAxis << endl;

	cout << "rotational matrix" << endl;
	cout << rot << endl;

	cout << "translation matrix" << endl;
	cout << trans << endl;

	return Mat4::mul(rot, trans);
}

ostream & operator<<(ostream & o, const Mat4 & m)
{
	for (int y=0; y < 16; y++)
	{
		if (y % 4 == 0)
			o << endl;

		o.width(10);
		o <<  m(0, y) << "  ";

	}

	return o;
}
