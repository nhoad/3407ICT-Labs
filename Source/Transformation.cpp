#include "Transformation.h"
#include <iostream>
using std::cout;
using std::endl;

Mat4::Mat4()
{
	for (int i=0; i < 16; i++)
		data[i] = (i % 5 == 0) ? 1 : 0;
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

int main(void)
{
	Mat4 m;

	cout << m(0, 0) << endl;
	cout << m(1, 1) << endl;
	cout << m(2, 2) << endl;
	cout << m(3, 3) << endl;

	cout << endl;
	m(0, 1) = 5;

	cout << m(0, 0) << endl;
	cout << m(0, 1) << endl;
	cout << m(0, 2) << endl;
	cout << m(0, 3) << endl;

	Mat4 n = m;

	cout << "new: " << endl;
	cout << n(0, 1) << endl;
}


