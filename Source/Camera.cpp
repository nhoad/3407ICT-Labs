#include "gl.h"
#include "glut.h"
#include "Camera.h"

#ifndef PI
#define PI 3.14159265
#endif

#include <cmath>

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::load()
{
	float xpos = position(0);
	float ypos = position(1);
	float zpos = position(2);

	float xRot = rotation(0);
	float yRot = rotation(1);

	Mat4 m = Mat4::translate(-xpos, -ypos, -zpos);
	m *= Mat4::rotateX(xRot);
	m *= Mat4::rotateY(yRot);

	glLoadMatrixf(m);

//	glLoadIdentity();
//	gluLookAt(xpos, ypos, zpos, target(0), target(1), target(2), up(0), up(1), up(2));
}

void Camera::setUp(Vec4 v)
{
	up = v;
}

void Camera::setTarget(Vec4 v)
{
	target = v;
}

void Camera::setPosition(Vec4 v)
{
	position = v;
}

void Camera::move(int direction)
{
	float xRot = rotation(0), yRot = rotation(1);

	switch (direction)
	{
		case FORWARD:
			position(0) += sin(yRot / 180.0 * PI);
			position(1) -= sin(xRot / 180.0 * PI);
			position(2) -= cos(yRot / 180.0 * PI);
			break;
		case BACKWARD:
			position(0) -= sin(yRot / 180.0 * PI);
			position(1) += sin(xRot / 180.0 * PI);
			position(2) += cos(yRot / 180.0 * PI);
			break;

		case LEFT:
			position(0) -= (cos(yRot / 180.0 * PI));
			position(2) -= (sin(yRot / 180.0 * PI));
			break;

		case RIGHT:
			position(0) += (cos(yRot / 180.0 * PI));
			position(2) += (sin(yRot / 180.0 * PI));
			break;
	}
}

void Camera::setSpeed(float speed)
{
	this->speed = speed;
}

void Camera::look(int direction)
{
	switch (direction)
	{
		case LEFT:
			target(0) -= speed;
			break;

		case RIGHT:
			target(0) += speed;
			break;

		case UP:
			rotation(0) += speed;
			break;

		case DOWN:
			rotation(0) -= speed;
			break;

	}
}
