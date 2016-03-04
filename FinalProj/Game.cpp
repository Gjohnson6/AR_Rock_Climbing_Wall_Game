#include "Game.h"

Game::Game(int width, int height)
{
	this->width = width;
	this->height = height;
}


Game::~Game()
{
}

void Game::init()
{
}

int Game::AddMarker(int x, int y)
{
	int retVal = -1;
	if (this->markers.size() < 9)
	{
		HitMarker mark = HitMarker(this->markers.size() + 1, x,  y);
		this->markers.push_back(mark);
		retVal = mark.getNum() - 1;
	}

	return retVal;
}


void Game::DetectHit(Mat thresholdImage)
{
	if (currToHit < markers.size())
	{
		Scalar colour = thresholdImage.at<uchar>(Point(markers[currToHit].getX(), thresholdImage.size().height - markers[currToHit].getY()));
		if (colour.val[0] == 255)
		{
			if (currToHit == 0)
			{
				startTime = clock();
			}
			markers[currToHit].getHit();
			currToHit++;
			if (currToHit == markers.size())
			{
				currTime = clock() - startTime;
				finished = true;
			}
		}
	}
	if (currToHit != 0 && !finished)
	{
		currTime = clock() - startTime;
	}
}

void Game::Reset()
{
	for (int i = 0; i < markers.size(); i++)
	{
		markers[i].Reset();
		startTime = 0;
		currTime = 0;
	}
	finished = false;
	currToHit = 0;
}

void Game::DisplayTimer()
{
	glPushMatrix();
	if (finished)
	{
		glColor3d(0, 1, 0);
	}
	else
	{
		glColor3d(1, 1, 1);
	}
	glScaled(.2, .2, .2);

	stringstream strTime;
	strTime << setprecision(2) << to_string((float)currTime / CLOCKS_PER_SEC);
	//string strTime = to_string((float)currTime / CLOCKS_PER_SEC);
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *)strTime.str().c_str());
	glPopMatrix();
}

//When deleting markers, change the markers' numbers to correspond
void Game::DeleteMarker(int x, int y)
{
	cout << "DELETE" << endl;
	for (int i = 0; i < markers.size(); i++)
	{
		if (markers[i].DetectMouse(x, y))
		{
			markers.erase(markers.begin() + i);
			for (int i = 0; i < markers.size(); i++)
			{
				markers[i].setNum(i + 1);
			}
			break;
		}
	}
}