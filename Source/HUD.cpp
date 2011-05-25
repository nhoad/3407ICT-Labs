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
	glOrtho(0.0f, width, 0.0f, height, 1, 10);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void HUD::reset()
{
	// return to our 3d modelview matrices
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
