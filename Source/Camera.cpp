#include "glut.h"

#include "Camera.h"

#include <cmath>
#include <iostream>
using std::cout;
using std::endl;

#include "Mat4.h"

#ifndef PI
#define PI 3.14159265
#endif

Camera::Camera(int mode)
{
	this->mode = mode;

	switch (mode)
	{
		case FPS:
			cout << "FPS mode enabled" << endl;
			break;
		case THIRD_PERSON:
			cout << "third person follow mode enabled" << endl;
			break;
	}
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

	//cout << "x rot: " << xRot << endl;
	//cout << "y rot: " << yRot << endl;

	Mat4 m = Mat4::translate(-xpos, -ypos, -zpos);
	m *= Mat4::rotateY(yRot);
	m *= Mat4::rotateX(xRot);

	glMultMatrixf(m);

//	glMultMatrixf(Mat4::lookAt(position, target, up));
}

void Camera::setUp(Vec4 v)
{
	//up = v;
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
			position(0) += sin(yRot / 180.0 * PI) * speed;
			position(1) -= sin(xRot / 180.0 * PI) * speed;
			position(2) -= cos(yRot / 180.0 * PI) * speed;
			break;
		case BACKWARD:
			position(0) -= sin(yRot / 180.0 * PI) * speed;
			position(1) += sin(xRot / 180.0 * PI) * speed;
			position(2) += cos(yRot / 180.0 * PI) * speed;
			break;

		case LEFT:
			position(0) -= (cos(yRot / 180.0 * PI)) * speed;
			position(2) -= (sin(yRot / 180.0 * PI)) * speed;
			break;

		case RIGHT:
			position(0) += (cos(yRot / 180.0 * PI)) * speed;
			position(2) += (sin(yRot / 180.0 * PI)) * speed;
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
			rotation(1) -= speed;
			break;

		case RIGHT:
			rotation(1) += speed;
			break;

		case UP:
			rotation(0) += speed;
			break;

		case DOWN:
			rotation(0) -= speed;
			break;

	}
}

void Camera::handleMouse(int x, int y)
{
	rotation(0) += y;
	rotation(1) += x;

	if (rotation(1) > 360.0)
		rotation(1) -= 360.0;

	if (rotation(0) > 80.0)
		rotation(0) = 80.0;

	if (rotation(0) < -30.0)
		rotation(0) = -30.0;

}
