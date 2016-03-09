#include "Button.h"
 
Button::~Button()
{
}

bool Button::DetectClick(int x, int y)
{
	bool retVal = false;
	if (x > left && x < right && y > bottom && y < top)
	{
		retVal = true;
	}	
	return retVal;
}

void Button::Draw()
{
	glColor3d(1.0, 1.0, 1.0);
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex2i(left, bottom);
	glVertex2i(right, bottom);

	glVertex2i(right, bottom);
	glVertex2i(right, top);
	
	glVertex2i(right, top);
	glVertex2i(left, top);
	
	glVertex2i(left, top);
	glVertex2i(left, bottom);
	
	glEnd();
	glTranslatef(left, bottom + 4, 0);
	glScaled(.2, .2, .2);

	glutStrokeString(GLUT_STROKE_ROMAN, (const unsigned char *)name.c_str());
	glPopMatrix();
}
