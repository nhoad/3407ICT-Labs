#include <iostream>
#pragma once

class Vec3
{
	public:
		float data[3];

		/**
			Constructor.

			\param x value of first element.
			\param y value of second element.
			\param z value of third element.
		*/
		Vec3(float x=0, float y=0, float z=0);

		/**
		  Constructur.

		  \param data float[3] containing points
		*/
		Vec3(float * data);

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
			\return the result of subtracting v from the current Vec3.
		*/
		Vec3 operator-(const Vec3 & v) const;

		/**
			Cross product of two Vectors.

			\param v the other vector to use for a cross product.
			\return cross product of this Vec3 with v.
		*/
		Vec3 operator*(const Vec3 & v) const;

		/**
			divide vector components by a float.

			\param l the float to divide components on
			\return result of dividing components by l.
		*/
		Vec3 operator/(const float& l) const;

		/**
			returns the length of the vector.

			\return square root of the vector multiplied by itself
		*/
		float length();

		/**
			Normalised form of current Vec3 object.

			\return normalised version of this object.
		*/
		Vec3 normalised();

		/**
			Nice output!
		*/
		friend std::ostream & operator<<(std::ostream & o, const Vec3 & m);

};
