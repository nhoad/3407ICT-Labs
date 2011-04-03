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

Mat4 Mat4::operator*(const Mat4 & m)
{
	Mat4 result(0.0);

	int row = 0;
	for (int row=0; row < 4; row++)
		for (int column=0; column < 4; column++)
		{
			for (int i=0; i < 4; i++)
				result(row, column) += (*this)(row, i) * m(i, column);
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
	Mat4 m, n;

	m(0, 1) = 5;
	n(0, 1) = 9;
	cout << m << endl;
	cout << n << endl;

	Mat4 r = m * n;

	cout << r << endl;

}


