#include "Transformation.h"
#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

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

ostream & operator<<(ostream & o, const Mat4 & m)
{
	for (int y=0; y < 16; y++)
		o << m(0, y) << endl;

	return o;
}

int main(void)
{
	Mat4 m;

	Vec4 a(4, 15, 3);

	cout << Mat4::mul(m, a) << endl;
}


