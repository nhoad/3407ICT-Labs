#include "HUD.h"
#include "glew.h"

HUD::HUD(int height, int width)
{
	this->height = height;
	this->width = width;
}

HUD::~HUD() {}

void HUD::draw()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void HUD::reset()
{
	// return to our 3d modelview matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
