#include "gl.h"
#include "glut.h"
#include "Camera.h"
#include <iostream>
using std::cout;
using std::endl;

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

	glLoadIdentity();
	gluLookAt(xpos, ypos, zpos, target(0), target(1), target(2), up(0), up(1), up(2));
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
	switch (direction)
	{
		case FORWARD:
			position(2) -= speed;
			break;

		case BACKWARD:
			position(2) += speed;
			break;

		case LEFT:
			position(0) -= speed;
			break;

		case RIGHT:
			position(0) += speed;
			break;

		case UP:
			position(1) += speed;
			break;

		case DOWN:
			position(1) -= speed;
			cout << "down" << endl;
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
			target(1) += speed;
			break;

		case DOWN:
			target(1) -= speed;
			break;

	}
}
