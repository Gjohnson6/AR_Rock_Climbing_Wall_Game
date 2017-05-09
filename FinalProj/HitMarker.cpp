#include "HitMarker.h"
#include "GameState.h"

HitMarker::HitMarker(int num, int x, int y)
{
	this->num = num;
	this->x = x;
	this->y = y;
	this->radius = 20;
}

HitMarker::~HitMarker()
{
}

void HitMarker::move(int x, int y)
{
	this->x = x;
	this->y = y;
}

void HitMarker::Draw()
{	

	glColor3d(1., 1., 1.);
	
	glPushMatrix();
	glTranslatef((float) this->x, (float) this->y, 0.f);

	GLUquadric* quadric;
	quadric = gluNewQuadric();
	gluDisk(quadric, 0, this->radius, 50, 3);
	//hit animation
	if (hit)
	{
		glColor3d(0., 1., 0.);
		if (this->radius < 60 && !dec)
		{
			this->radius += 2 * GameState::GetInstance()->getScale();
			if (this->radius >= 60)
			{
				this->radius = 60;
				dec = true;
			}
		}
		else if(dec)
		{
			if (this->radius > 40)
			{
				this->radius -= 2 * GameState::GetInstance()->getScale();
			}
			
			if(this->radius < 40)
			{
				this->radius = 40;
			}
			gluDisk(quadric, this->radius, 60.0, 50, 3);
		}
	}
	glTranslatef(-10., -10., -1.);
	glColor3d(0., 0., 0.);
	glScaled(.2, .2, .2);
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *)to_string(num).c_str());
	gluDeleteQuadric(quadric);

	glPopMatrix();
}

bool HitMarker::DetectMouse(int x, int y)//Detects if the mouse is over the HitMarker, used for dragging existing hitmarkers
{
	return (glm::distance(glm::vec2(x, y), glm::vec2(this->x, this->y)) < this->radius);
}

int HitMarker::getNum()
{
	return num;
}

void HitMarker::setNum(int num)
{
	this->num = num;
}

void HitMarker::getHit()
{
	this->hit = true;
}

int HitMarker::getX()
{
	return this->x;
}

int HitMarker::getY()
{
	return this->y;
}

void HitMarker::Reset()
{
	this->hit = false;
	this->dec = false;
	this->radius = 20;
}