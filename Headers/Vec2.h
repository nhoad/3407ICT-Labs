#pragma once
#include <iostream>

class Vec2
{
	public:
		float data[2];

		/**
			Constructor.

			\param x value of first element.
			\param y value of second element.
		*/
		Vec2(float x=0, float y=0);

		/**
		  Constructur.

		  \param data float[2] containing points
		*/
		Vec2(float * data);

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
			\return the result of subtracting v from the current Vec2.
		*/
		Vec2 operator-(const Vec2 & v) const;

		/**
			Cross product of two Vectors.

			\param v the other vector to use for a cross product.
			\return cross product of this Vec2 with v.
		*/
		Vec2 operator*(const Vec2 & v) const;

		/**
			divide vector components by a float.

			\param l the float to divide components on
			\return result of dividing components by l.
		*/
		Vec2 operator/(const float& l) const;

		/**
			returns the length of the vector.

			\return square root of the vector multiplied by itself
		*/
		float length();

		/**
			Normalised form of current Vec2 object.

			\return normalised version of this object.
		*/
		Vec2 normalised();

		/**
			Nice output!
		*/
		friend std::ostream & operator<<(std::ostream & o, const Vec2 & m);

};
