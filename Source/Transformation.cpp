#include "Transformation.h"
#define PI 3.14159265
#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

#include <cmath>

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

ostream & operator<<(ostream & o, const Vec4 & v)
{
	o << "x: " << v.x << endl;
	o << "y: " << v.y << endl;
	o << "z: " << v.z << endl;
	o << "w: " << v.w << endl;

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
	Mat4 result(0.0);
	for (int row=0; row < 4; row++)
		for (int column=0; column < 4; column++)
			for (int i=0; i < 4; i++)
				result(row, column) += m(row, i) * n(i, column);
	return result;
}

Vec4 Mat4::mul(const Mat4& m, const Vec4& v)
{
	Vec4 result(0, 0, 0, 0);

	for (int row=0; row < 4; row++)
		for (int i=0; i < 4; i++)
		{
			cout << "multiplying " << m(row, i) << " by " << v(i) << endl;
			result(row) += m(row, i) * v(i);
		}

	return result;
}

Mat4 Mat4::translate(float x, float y, float z)
{
	Mat4 result;

	result(3, 0) = x;
	result(3, 1) = y;
	result(3, 2) = z;


	return result;
}

Mat4 Mat4::rotateX(float degree)
{
	Mat4 result = Mat4::translate(0.0, 0.0, 0.0);

	degree = (degree * PI) / 180;

	const float c = cos(degree);
	const float s = sin(degree);

	result(1, 1) = c;
	result(1, 2) = s;
	result(2, 1) = -s;
	result(2, 2) = c;

	return result;

}

Mat4 Mat4::rotateY(float degree)
{
	Mat4 result;

	degree = (degree * PI) / 180;

	const float c = cos(degree);
	const float s = sin(degree);

	result(0, 0) = c;
	result(0, 2) = -s;
	result(2, 0) = s;
	result(2, 2) = c;

	return result;
}

Mat4 Mat4::rotateZ(float degree)
{
	Mat4 result;

	degree = (degree * PI) / 180;

	const float c = cos(degree);
	const float s = sin(degree);

	result(0, 0) = c;
	result(1, 0) = -s;
	result(0, 1) = s;
	result(1, 1) = c;

	return result;
}

Mat4 Mat4::scale(float x, float y, float z)
{
	Mat4 result;

	result(0, 0) = x;
	result(1, 1) = y;
	result(2, 2) = z;

	return result;
}

ostream & operator<<(ostream & o, const Mat4 & m)
{
	for (int y=0; y < 16; y++)
		o << m(0, y) << endl;

	return o;
}

/*int main(void)
{
	Vec4 a(4, 15, 3);

	cout << Mat4::mul(Mat4::scale(0.5, 0.5, 0.5), a) << endl;

//	cout << Mat4::mul(m, a) << endl;
}

*/
