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
	if (markers.size() < 9)
	{
		HitMarker mark = HitMarker(markers.size() + 1, x,  y);
		markers.push_back(mark);
		retVal = mark.getNum() - 1;
	}
	return retVal;
}


void Game::DetectHit(Mat thresholdImage)
{
	if (currToHit < markers.size() && gameStarted)
	{
		Scalar colour = thresholdImage.at<uchar>(Point(markersRealPos[currToHit].x, thresholdImage.size().height - markersRealPos[currToHit].y));
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

	stringstream strTime;
	strTime << setprecision(2) << to_string((float)currTime / CLOCKS_PER_SEC) << "   " << markers.size();
	//string strTime = to_string((float)currTime / CLOCKS_PER_SEC);
	//glutStrokeString(GLUT_ _MONO_ROMAN, (const unsigned char *)strTime.str().c_str());
	glRasterPos2i(GameState::GetInstance()->getWidth() / 8 , GameState::GetInstance()->getHeight() / 2);
	glScaled(30, 30, 30);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)strTime.str().c_str());
	glPopMatrix();
}

void Game::SetMap(Map map)
{
	edit = false;
	map = map;
}

void Game::Clear()
{
	markers.clear();
	currTime = 0;
	startTime = 0;
}

//This converts the onscreen positions of the markers to the position they will be in the camera's eye
void Game::convertMarkersToRealPos()
{
	markersRealPos.clear();

	cout << "OLD VALUES: " << endl;
	for (auto marker : markers)
	{
		Point2i point(marker.getX(), marker.getY());
		markersRealPos.push_back(point);
		cout << point.x << "," << point.y << endl;
	}
	try
	{
		for (int i = 0; i < markersRealPos.size(); i++)
		{
			markersRealPos[i].y = GameState::GetInstance()->getHeight() - markersRealPos[i].y;
		}

		cv::Mat dst = cv::Mat(markersRealPos, CV_32FC1);
		//warpPerspective(cv::Mat(markersRealPos), cv::Mat(markersRealPos), GameState::GetInstance()->getTransform(), dst.size());
		perspectiveTransform(dst, dst, GameState::GetInstance()->getPointTransform());

		dst.copyTo(markersRealPos);

		Point2f p = GameState::GetInstance()->getBottomLeft();
		p.y = GameState::GetInstance()->getHeight() - p.y;

		for (int i = 0; i < markersRealPos.size(); i++)
		{
			markersRealPos[i].y = GameState::GetInstance()->getHeight() - markersRealPos[i].y;
		}
	}
	catch (Exception e)
	{
		cout << e.msg << endl;
	}

	cout << "NEW VALUES: " << endl;
	for (auto point : markersRealPos)
	{
		cout << point.x << "," << point.y << endl;
	}
	//cv::transform(markersRealPos, markersRealPos, GameState::GetInstance()->getTransform());
}

//When deleting markers, change the markers' numbers to correspond
void Game::DeleteMarker(int x, int y)
{
	if (edit)
	{
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
}

void Game::DisplayFunc()
{
	if(countNonZero(prevGray) < 1)
	{
		currFrame = GameState::GetInstance()->readAndResize();
		cv::cvtColor(currFrame, currGray, COLOR_BGR2GRAY);
	}
	
	prevGray = currGray.clone();
	currFrame = GameState::GetInstance()->readAndResize();
	
	//cv::warpPerspective(currFrame, currFrame, GameState::GetInstance()->getTransform(), currFrame.size(), INTER_LINEAR, BORDER_CONSTANT);
	//convert frame2 to gray scale for frame differencing
	cv::cvtColor(currFrame, currGray, COLOR_BGR2GRAY);

	//perform frame differencing with the sequential images. This will output an "intensity image"
	//do not confuse this with a threshold image, we will need to perform thresholding afterwards.
	
	cv::absdiff(currGray, prevGray, differenceImage);
	//cv::blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
	//threshold intensity image at a given sensitivity value
	cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

	//use blur() to smooth the image, remove possible noise and
	//increase the size of the object we are trying to track. (Much like dilate and erode)

	//threshold again to obtain binary image from blur output
	//cv::threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

	if (debugMode == true) {
		glRasterPos2i(0, 0);
		Mat image;

		switch (debugState)
		{
		case(NORM) :
			currFrame.copyTo(image);
			//warpPerspective(image, image, GameState::GetInstance()->getCamTransform(), image.size());
			cv::flip(image, image, 0);
			glDrawPixels(image.size().width, image.size().height, GL_BGR_EXT, GL_UNSIGNED_BYTE, image.ptr());

			glBegin(GL_POINTS);
			for (auto point : markersRealPos)
			{
				glVertex2i(point.x, point.y);
			}
			glEnd();
			break;
		case(THRESH):
			cv::flip(thresholdImage, image, 0);
			warpPerspective(image, image, GameState::GetInstance()->getCamTransform(), image.size());
			glDrawPixels(image.size().width, image.size().height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image.ptr());
			break;
		case(DIFF):
			cv::flip(differenceImage, image, 0);
			warpPerspective(image, image, GameState::GetInstance()->getCamTransform(), image.size());
			glDrawPixels(image.size().width, image.size().height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image.ptr());
			break;
		}

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
	if (edit)
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
}

void Game::MouseMotionFunc(int& x, int& y)
{
	if (edit)
	{
		if (mouseHeld && currMarker >= 0)
		{
			markers[currMarker].move(x, GameState::GetInstance()->getHeight() - y);
		}
	}
}

void Game::KeyboardFunc(unsigned char & c, int& x, int& y)
{
	switch (c)
	{
	case('d') :
		debugMode = !debugMode;
		debugState = NORM;
		break;
	case('r') :
		Reset();
		break;
	case('t') :
		debugState = THRESH;
		break;
	case('f') :
		debugState = DIFF;
		break;
	case('p') :
		debugState = POINT;
		break;
	case('e') :
		edit = !edit;
		break;
	case('s') :
		convertMarkersToRealPos();
		gameStarted = true;
		break;
	case(27) :
		Clear();
		GameState::GetInstance()->getGMenu()->setState(GameMenu::MENU);
		break;
	}
}