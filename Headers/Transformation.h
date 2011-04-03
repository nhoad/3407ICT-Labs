#include <iostream>

class Vec4;

class Mat4
{
	float data[16];

	public:
		Mat4();
		Mat4(float * values);
		Mat4(float defaultValue);
		float& operator()(int x, int y);
		float operator()(int x, int y) const;
		Mat4& operator=(const Mat4& m);
		friend std::ostream & operator<<(std::ostream & o, const Mat4 & m);

		static Mat4 mul(const Mat4& m, const Mat4& n);
		static Vec4 mul(const Mat4& m, const Vec4& v);
};

class Vec4
{
	public:
		float x, y, z, w;
		Vec4(float x=0, float y=0, float z=0, float w=1) : x(x), y(y), z(z), w(w) { };

		float& operator()(int x);
		float operator()(int x) const;

		friend std::ostream & operator<<(std::ostream & o, const Mat4 & m);

};
