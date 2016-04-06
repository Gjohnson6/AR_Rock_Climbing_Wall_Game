#include "Game.h"
#include "GameState.h"
Game::Game()
{
}

Game::~Game()
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
		glColor3d(0.0, 1.0, 0.0);
	}
	else
	{
		glColor3d(1.0, 1.0, 1.0);
	}
	glScaled(.2, .2, .2);

	stringstream strTime;
	strTime << setprecision(2) << to_string((float)currTime / CLOCKS_PER_SEC) << "   " << markers.size();
	//string strTime = to_string((float)currTime / CLOCKS_PER_SEC);
	//glutStrokeString(GLUT_ _MONO_ROMAN, (const unsigned char *)strTime.str().c_str());
	glRasterPos2i(4, 4);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)strTime.str().c_str());
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

void Game::DisplayFunc()
{
	if(countNonZero(prevGray) < 1)
	{
		GameState::GetInstance()->getCap().read(currFrame);
		cv::cvtColor(currFrame, currGray, COLOR_BGR2GRAY);
	}

	prevGray = currGray.clone();
	GameState::GetInstance()->getCap().read(currFrame);
	
	cv::warpPerspective(currFrame, currFrame, GameState::GetInstance()->getTransform(), currFrame.size(), INTER_LINEAR, BORDER_CONSTANT);
	//convert frame2 to gray scale for frame differencing
	cv::cvtColor(currFrame, currGray, COLOR_BGR2GRAY);

	//perform frame differencing with the sequential images. This will output an "intensity image"
	//do not confuse this with a threshold image, we will need to perform thresholding afterwards.
	
	cv::absdiff(currGray, prevGray, differenceImage);
	cv::blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
	//threshold intensity image at a given sensitivity value
	cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

	//use blur() to smooth the image, remove possible noise and
	//increase the size of the object we are trying to track. (Much like dilate and erode)

	//threshold again to obtain binary image from blur output
	//cv::threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

	if (debugMode == true) {
		//show the difference image and threshold image
		//cv::imshow("Difference Image", differenceImage);
		//cv::imshow("Threshold Image", thresholdImage);
		//cv::imshow("Gray image 1", currGray);
		//cv::imshow("Prev", prevGray); 
		glRasterPos2i(0, 0);
		Mat image;

		if (thresh)
		{
			cv::flip(thresholdImage, image, 0);
			glDrawPixels(image.size().width, image.size().height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image.ptr());
		}
		else if (diff)
		{
			cv::flip(differenceImage, image, 0);
			glDrawPixels(image.size().width, image.size().height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image.ptr());
		}
		else 
		{
			cv::flip(currFrame, image, 0);
			glDrawPixels(image.size().width, image.size().height, GL_BGR_EXT, GL_UNSIGNED_BYTE, image.ptr());
		}

	}
	else {
		//if not in debug mode, destroy the windows so we don't see them anymore
		cv::destroyWindow("Difference Image");
		cv::destroyWindow("Threshold Image");
	}

	//Check if the current point has been hit
	DetectHit(thresholdImage);

	//Draw them arkers, this is done after the debugMode block so that the camera's view is displayed behind the markers.
	for (int i = 0; i < markers.size(); i++)
	{
		markers[i].Draw();
	}

	DisplayTimer();
}

void Game::MouseFunc(int& button, int& state, int& x, int& y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			bool dragging = false;
			for (auto mark : markers)
			{
				if (mark.DetectMouse(x, GameState::GetInstance()->getHeight() - y))
				{
					currMarker = mark.getNum() - 1;
					dragging = true;
				}
			}

			if (!dragging)
			{
				currMarker = AddMarker(x, GameState::GetInstance()->getHeight() - y);
			}

			mouseHeld = true;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			DeleteMarker(x, GameState::GetInstance()->getHeight() - y);
		}
	}
	else if (state == GLUT_UP)
	{
		mouseHeld = false;
	}
}

void Game::MouseMotionFunc(int& x, int& y)
{
	if (mouseHeld && currMarker >= 0)
	{
		markers[currMarker].move(x, GameState::GetInstance()->getHeight() - y);
	}
}

void Game::KeyboardFunc(unsigned char & c, int& x, int& y)
{
	switch (c)
	{
	case('d'):
		debugMode = !debugMode;
		break;
	case('r') :
		Reset();
		break;
	case('t') :
		thresh = !thresh;
		break;
	case('f') :
		diff = !diff;
		break;
	case(27) :
		GameState::GetInstance()->getGMenu()->setState(GameMenu::MENU);
		break;
	}
}