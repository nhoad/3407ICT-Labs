#include <iostream>

#pragma once
class Vec4;

class Mat4
{
	private:
		float data[16];

	public:

		Mat4();
		Mat4(float * values);
		Mat4(float defaultValue);
		float& operator()(int x, int y);
		float operator()(int x, int y) const;
		Mat4& operator=(const Mat4& m);
		operator float*();
		friend std::ostream & operator<<(std::ostream & o, const Mat4 & m);

		static Mat4 mul(const Mat4& m, const Mat4& n);
		static Vec4 mul(const Mat4& m, const Vec4& v);

		static Mat4 translate(float x, float y, float z);
		static Mat4 rotateX(float degree);
		static Mat4 rotateY(float degree);
		static Mat4 rotateZ(float degree);
		static Mat4 scale(float x, float y, float z);
		static Mat4 lookAt(Vec4 & camera, Vec4 & target, Vec4 & up);
		static Mat4 perspectiveMatrix(float fieldOfView, float aspectRatio, float near, float far);
		static Mat4 perspectiveFrustrum(float fieldOfView, float aspectRatio, float near, float far);

};

class Vec4
{
	public:
		float data[4];
		Vec4(float x=0, float y=0, float z=0, float w=0);

		float& operator()(int x);
		float operator()(int x) const;

		/**
			Subtract one Vector from another.

			\param v the vector to subtract
			\return the result of subtracting v from the current Vec4.
		*/
		Vec4 operator-(const Vec4 & v) const;

		/**
			Cross product of two Vectors.

			\param v the other vector to use for a cross product.
			\return cross product of this Vec4 with v.
		*/
		Vec4 operator*(const Vec4 & v) const;

		/**
			divide vector components by a float.

			\param l the float to divide components on
			\return result of dividing components by l.
		*/
		Vec4 operator/(const float& l) const;

		float length();

		Vec4 normalised();

		friend std::ostream & operator<<(std::ostream & o, const Vec4 & m);

};
