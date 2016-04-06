#include "GameState.h"

void GameState::DisplayFunc()
{
	switch (currState)
	{
	case(MENU) :
		mMenu.DisplayFunc();
		break;
	case(GAME) :
		gMenu->DisplayFunc();
		break;
	case(CALIB) :
		calib.DisplayFunc();
		break;
	case(EXIT) :
		glutLeaveMainLoop();
		break;
	}
}

void GameState::MouseFunc(int & button, int & state, int & x, int & y)
{
	switch (currState)
	{
	case(MENU) :
		mMenu.MouseFunc(button, state, x, y);
		break;
	case(GAME) :
		gMenu->MouseFunc(button, state, x, y);
		break;
	case(CALIB) :
		calib.MouseFunc(button, state, x, y);
		break;

	}
}


void GameState::MouseMotionFunc(int & x, int & y)
{
}

void GameState::KeyboardFunc(unsigned char & c, int & x, int & y)
{
	switch (currState)
	{
	case(MENU) :
		break;
	case(GAME) :
		gMenu->KeyboardFunc(c, x, y);
		break;
	case(CALIB) :
		calib.KeyboardFunc(c, x, y);
		break;
	}
}

void GameState::SpecialFunc(int key, int x, int y)
{
	switch (currState)
	{
	case(MENU) :
		break;
	case(GAME) :
		break;
	case(CALIB) :
		calib.SpecialFunc(key, x, y);
		break;
	}
}

bool GameState::GLReturnedError(char * s)
{
	bool return_error = false;
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		cerr << s << ": " << gluErrorString(glerror) << endl;
	}

	return return_error;
}

int GameState::getHeight()
{
	return winHeight;
}

int GameState::getWidth()
{
	return winWidth;
}

void GameState::setState(int state)
{
	currState = state;
}

VideoCapture GameState::getCap()
{
	return cap;
}

void GameState::setTransform(Mat t)
{
	transform = t;
}

Mat GameState::getTransform()
{
	return transform;
}

Game GameState::getGame()
{
	return game;
}

GameMenu* GameState::getGMenu()
{
	return gMenu;
}

GameState::GameState()
{
	cap = VideoCapture(1);
	Point2f dst_vertices[4];
	dst_vertices[0] = Point(0, 0);
	dst_vertices[1] = Point(winWidth, 0);
	dst_vertices[2] = Point(winHeight, winWidth);
	dst_vertices[3] = Point(0, winWidth);

	transform = getPerspectiveTransform(dst_vertices, dst_vertices);

	gMenu = new GameMenu();
}


GameState::~GameState()
{
	delete gMenu;
}
