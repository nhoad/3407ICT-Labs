/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Nathan Hoad (nathan@getoffmalawn.com)
 * Student Number: s2754580
 */
#include <iostream>
#pragma once
class Vec4;

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
		Mat4& operator*=(Mat4 m);

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

/** Vec4 class repesenting a 1x4 matrix. */
class Vec4
{
	public:
		float data[4];

		/**
			Constructor.

			\param x value of first element.
			\param y value of second element.
			\param z value of third element.
			\param w value of last element.
		*/
		Vec4(float x=0, float y=0, float z=0, float w=0);

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

		/**
			returns the length of the vector.

			\return square root of the vector multiplied by itself
		*/
		float length();

		/**
			Normalised form of current Vec4 object.

			\return normalised version of this object.
		*/
		Vec4 normalised();

		/**
			Nice output!
		*/
		friend std::ostream & operator<<(std::ostream & o, const Vec4 & m);

};
