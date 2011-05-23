#pragma once
#include "Vec4.h"
#include "Vec3.h"

/**
	Mat4 class representing a 4x4 matrix.
*/
class Mat4
{
	private:
		float data[16];

	public:

		/**
			Default constructor. Creates an identity matrix.
		*/
		Mat4();

		/**
			Constructor. Creates a 4x4 matrix using predecided values.

			\param values the values to apply to Mat4.
		*/
		Mat4(float * values);

		/**
			Constructor. Creates a 4x4 matrix with every element set to a default value.

			\param defaultValue the value to set every element to.
		*/
		Mat4(float defaultValue);

		/**
			Single dimensional access to data

			\param x the index to return.
			\return data at position x.
		*/
		float& operator()(int x);

		/**
			Single dimensional access to data

			\param x the index to return.
			\return data at position x.
		*/
		float operator()(int x) const;

		/**
			Two dimensional access to data

			\param x x coordinate in matrix to return
			\param y y coordinate in matrix to return
			\return data at position x/y
		*/
		float& operator()(int x, int y);

		/**
			Two dimensional access to data

			\param x x coordinate in matrix to return
			\param y y coordinate in matrix to return
			\return data at position x/y
		*/
		float operator()(int x, int y) const;

		/**
			Replace all data in current matrix with data from m.

			\param m the matrix to get data from
			\return new matrix containing data from m.
		*/
		Mat4& operator=(const Mat4& m);

		/**
			Perform matrix multiplication.

			\param m matrix to multiply this against.
			\return this * m.
		*/
		Mat4 operator*(const Mat4& m);

		/**
			Perform matrix multiplication.

			\param m matrix to multiply this against.
			\return this * m.
		*/
		Mat4& operator*=(Mat4 & m);

		/**
			Overload * operator. Provides direct access to data.

			\return data.
		*/
		operator float*();

		/**
			Nice output! For cout, cerr etc.
		*/
		friend std::ostream & operator<<(std::ostream & o, const Mat4 & m);

		/**
			Perform matrix multiplication.

			\param m matrix first matrix.
			\param n matrix to multiply againt n.
			\return m * n.
		*/
		static Mat4 mul(const Mat4& m, const Mat4& n);

		/**
			Vector by matrix multiplication.

			\param m matrix to multiply.
			\param v Vec4 to multiply against.
			\return v * m.
		*/
		static Vec4 mul(const Mat4& m, const Vec4& v);

		/**
			Construct a translation matrix.

			\param x x coord to translate by.
			\param y y coord to translate by.
			\param y y coord to translate by.
			\return translation matrix with x, y and z at elements 12, 13 and 14 (0 based).
		*/
		static Mat4 translate(float x, float y, float z);

		/**
			Construct a translation matrix.

			\param coords the x, y and z coordinates to translate by.
			\return translation matrix with x, y and z at elements 12, 13 and 14 (0 based).
		*/
		static Mat4 translate(Vec3 & coords);

		/**
			Rotation matrix on x axis.

			\param angle the angle to rotate by on x axis.
			\return rotation matrix on x axis.
		*/
		static Mat4 rotateX(float angle);

		/**
			Rotation matrix on y axis.

			\param angle the angle to rotate by on y axis.
			\return rotation matrix on y axis.
		*/
		static Mat4 rotateY(float angle);

		/**
			Rotation matrix on z axis.

			\param angle the angle to rotate by on z axis.
			\return rotation matrix on z axis.
		*/
		static Mat4 rotateZ(float angle);

		/**
			Scaling matrix.

			\param x how much to scale on x axis.
			\param y how much to scale on y axis.
			\param z how much to scale on z axis.
			\return Scaling matrix from x, y and z values.
		*/
		static Mat4 scale(float x, float y, float z);

		/**
			Construct a camera

			\param camera the position of the camera.
			\param target what the camera should look at.
			\param up the up vector.
		*/
		static Mat4 lookAt(Vec4 & camera, Vec4 & target, Vec4 & up);

		/**
			Construct a perspective matrix.

			\param fieldOfView angle of view (45 is a nice value).
			\param aspectRatio aspect ratio of the window (typically width / height)
			\param near the near value
			\param far the far value
			\return description
		*/
		static Mat4 perspectiveMatrix(float fieldOfView, float aspectRatio, float near, float far);
};
