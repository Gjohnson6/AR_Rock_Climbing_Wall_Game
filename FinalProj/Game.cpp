#include "Game.h"
#include "GameState.h"
#include <windows.h>
#include <string>
#include <experimental\filesystem>

Game::Game()
{
	addButton(RETURN, "Return");
	addButton(LOAD, "Load");
	addButton(SAVE, "Save");
	addButton(CLEAR, "Clear");
	addButton(RESTART, "Restart");
	addButton(READY, "Ready");
	int rightSideWidth = GameState::GetInstance()->getWidth() * .75f;

	left = GameState::GetInstance()->getWidth() * .75f * .25f + GameState::GetInstance()->getWidth() * .25f;
	right = GameState::GetInstance()->getWidth() * .75f * .75f + GameState::GetInstance()->getWidth() * .25f;
	top = GameState::GetInstance()->getHeight() / 2 + 20;
	bottom = top - 40;
	width = right - left;

	filename = ".\\maps\\";
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
		Scalar color = thresholdImage.at<uchar>(Point(markersRealPos[currToHit].x, thresholdImage.size().height - markersRealPos[currToHit].y));
		if (color.val[0] > 0)
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
	strTime.precision(2);
	strTime << setprecision(2) << fixed;
	strTime << (float)currTime / CLOCKS_PER_SEC;
	glTranslated(GameState::GetInstance()->getWidth() / 8 , GameState::GetInstance()->getHeight() / 2, 0);
	glScaled(.5, .5, .5);
	string s = strTime.str();

	//shiftDist is how the decimal point stays in the middle.
	//It takes the width of the characters (they're fixed width, so it's the same for all characters), multiples it by how many are to the left of the decimal (s.length - 3)
	int shiftDist = -1 * glutStrokeWidth(GLUT_STROKE_MONO_ROMAN, '0') * (s.length() - 3);
	//Then translate that many pixels left
	glTranslated(shiftDist, 0, 0);
	
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)s.c_str());
	glPopMatrix();
}

void Game::SetMap(Map map)
{
	edit = false;
	map = map;
}

void Game::Clear()
{
	Unready();
	markers.clear();
	currTime = 0;
	startTime = 0;
}

void Game::Ready()
{
	convertMarkersToRealPos();
	gameStarted = true;
	edit = false;
	buttons.erase(buttons.end() - 1);
	addButton(UNREADY, "Unready");
}

void Game::Unready()
{
	Reset();
	edit = true;
	gameStarted = false;
	buttons.erase(buttons.end() - 1);
	addButton(READY, "Ready");
}

void Game::Save()
{
	Unready();
	displayState = DISP_SAVE;
}

void Game::Load()
{
	lMenu.Clear();
	vector<string> names;

	int i = 0;
	int count = 0;
	int maxButtonCount = (GameState::GetInstance()->getHeight() - 20) / 60;
	for (auto file : experimental::filesystem::directory_iterator(".\\maps\\"))
	{
		count++;
		stringstream s;
		s << file;
		cout << file << endl;
		lMenu.addButton(count % maxButtonCount, s.str());
		s.clear();
	}

	displayState = DISP_LOAD;
}

void Game::SaveKeyboardFunc(char c)
{
	Map map(markers);
	switch (c)
	{
	case('\b') :
		//>7 to keep "./maps/"intact
		if (filename.length() > 7)
		{
			filename.pop_back();
		}
		break;
	case('\r') :
		//Save the file
		map.SaveMap(filename);
	case(27) :
		filename = ".\\maps\\";
		displayState = DISP_GAME;
		edit = true;
		break;
	default:
		filename.push_back(c);
		//dispFilename.push_back(c);
		break;
	}
}

void Game::GameKeyboardFunc(char c)
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
		Return();
		break;
	}
}

void Game::GameMouseFunc(int& button, int & x, int & y)
{
	if (button == GLUT_LEFT_BUTTON)
			{
				//If the mouse is in the marker area check if we're trying to create a new marker or move a marker
				//Also, check if the map is allowed to be edited
				if (x > GameState::GetInstance()->getWidth() / 4 && edit)
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

					if (!dragging && x > GameState::GetInstance()->getWidth() / 4)
					{
						currMarker = AddMarker(x, GameState::GetInstance()->getHeight() - y);
					}

					mouseHeld = true;
				}
				//Otherwise it's in the menu area, so we need to check if it's clicking on any of the buttons
				else
				{
					switch (DetectClick(x, GameState::GetInstance()->getHeight() - y))
					{
					case(READY) :
						Ready();
						break;
					case(UNREADY) :
						Unready();
						break;
					case(RESTART) :
						Reset();
						break;
					case(CLEAR) :
						Clear();
						break;
					case(LOAD) :
						//Open file menu, load Map object, take in button positions, turn off editing (maybe have option to turn it back on?)
						Load();
						break;
					case(SAVE) :
						//Make map object using markers,  open file menu, save
						Save();
						break;
					case(RETURN) :
						Return();
						break;
					default:
						break;
					}
				}
			}
			else if (button == GLUT_RIGHT_BUTTON)
			{
				DeleteMarker(x, GameState::GetInstance()->getHeight() - y);
			}
}

void Game::LoadMouseFunc(int& button, int & x, int & y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		string file;
		if (( file = lMenu.detectClickReturnString(x, GameState::GetInstance()->getHeight() - y)) != "" && file != "Previous Page" && file != "Next Page")
		{
			cout << file << endl;
			Map loadMap;
			loadMap.LoadMap(file);
			markers.clear();

			markers = loadMap.getMarkers();
			displayState = DISP_GAME;
		}
		else if (file == "Previous Page")
		{
			lMenu.PageLeft();
		}
		else if (file == "Next Page")
		{
			lMenu.PageRight();
		}

		if (DetectClick(x, GameState::GetInstance()->getHeight() - y) == RETURN)
		{
			displayState = DISP_GAME;
		}
	}
}

//This converts the onscreen positions of the markers to the position they will be in the camera's eye
void Game::convertMarkersToRealPos()
{
	markersRealPos.clear();

	for (auto marker : markers)
	{
		Point2i point(marker.getX(), marker.getY());
		markersRealPos.push_back(point);
	}

	try
	{
		for (int i = 0; i < markersRealPos.size(); i++)
		{
			markersRealPos[i].y = GameState::GetInstance()->getHeight() - markersRealPos[i].y;
		}

		cv::Mat dst = cv::Mat(markersRealPos, CV_32FC1);
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

	for (auto point : markersRealPos)
	{
		cout << point.x << "," << point.y << endl;
	}
	//cv::transform(markersRealPos, markersRealPos, GameState::GetInstance()->getTransform());
}

void Game::Return()
{
	GameState::GetInstance()->setState(GameState::MENU);
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

	switch (displayState)
	{
		case(DISP_GAME):	
			//Check if the current point has been hit
			DetectHit(thresholdImage);

			//Draw them arkers, this is done after the debugMode block so that the camera's view is displayed behind the markers.
			for (int i = 0; i < markers.size(); i++)
			{
				markers[i].Draw();
			}
			break;
		case(DISP_LOAD) :
			lMenu.DisplayFunc();
			break;
		case(DISP_SAVE) :
			glPushMatrix();
			int rightSideWidth = GameState::GetInstance()->getWidth() * .75f;
			int midPos = rightSideWidth / 2 + GameState::GetInstance()->getWidth() / 4;

			glTranslated(midPos, GameState::GetInstance()->getHeight() / 2 + 80, 0);
			glScaled(.5, .5, .5);
			string s = "Enter in a filename";
			int shiftDist = -1 * (glutStrokeWidth(GLUT_STROKE_MONO_ROMAN, '0') * (s.length() / 2 + .5)) ;
			//Then translate that many pixels left
			glTranslated(shiftDist, 0, 0);

			//glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)strTime.str().c_str());
			glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char*)s.c_str());

			glPopMatrix();

			glPushMatrix();

			glColor3d(1.0, 1.0, 1.0);
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
			glTranslatef((float)left, (float)bottom + 4.f, 0.f);
			glScaled(.2, .2, .2);
			string dispFilename = filename.substr(7, filename.length() - 1);
			int w = glutStrokeWidth(GLUT_STROKE_MONO_ROMAN, '0') * .2;
			if (dispFilename.length() * w > width)
			{
				dispFilename = filename.substr(filename.length() - width / w, filename.length() - 1);
			}
			glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *)dispFilename.c_str());

			glPopMatrix();


			break;
	}

	for (auto button : buttons)
	{
		button.Draw();
	}

	DisplayTimer();
}

void Game::MouseFunc(int& button, int& state, int& x, int& y)
{
	if (state == GLUT_DOWN)
	{
		switch (displayState)
		{
		case(DISP_GAME) :
			GameMouseFunc(button, x, y);
			break;
		case(DISP_LOAD) :
			LoadMouseFunc(button, x, y);
			break;
		}
	}
	else if (state == GLUT_UP)
	{
		mouseHeld = false;
	}
}

void Game::MouseMotionFunc(int& x, int& y)
{
	if (edit)
	{
		if (mouseHeld && currMarker >= 0)
		{
			//Don't let the user move the marker into the menu/timer area
			int newX = x > GameState::GetInstance()->getWidth() / 4 ? x : GameState::GetInstance()->getWidth() / 4;
			markers[currMarker].move(newX, GameState::GetInstance()->getHeight() - y);
		}
	}
}

void Game::KeyboardFunc(unsigned char & c, int& x, int& y)
{
	switch (displayState)
	{
	case(DISP_GAME) :
		GameKeyboardFunc(c);
		break;
	case(DISP_SAVE) :
		SaveKeyboardFunc(c);
		break;
	}
}