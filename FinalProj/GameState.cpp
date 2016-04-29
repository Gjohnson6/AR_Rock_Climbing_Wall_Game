#include "GameState.h"

void GameState::DisplayFunc()
{
	prevFrame = currFrame;
	currFrame = clock();
	switch (currState)
	{
	case(MENU) :
		mMenu->DisplayFunc();
		break;
	case(GAME) :
		game->DisplayFunc();
		//gMenu->DisplayFunc();
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
		mMenu->MouseFunc(button, state, x, y);
		break;
	case(GAME) :
		game->MouseFunc(button, state, x, y);
		//gMenu->MouseFunc(button, state, x, y);
		break;
	case(CALIB) :
		calib.MouseFunc(button, state, x, y);
		break;
	}
}


void GameState::MouseMotionFunc(int & x, int & y)
{
	switch (currState)
	{
	case(GAME) :
		game->MouseMotionFunc(x, y);
		//gMenu->MouseMotionFunc(x, y);
	}
}

void GameState::KeyboardFunc(unsigned char & c, int & x, int & y)
{
	switch (currState)
	{
	case(MENU) :
		break;
	case(GAME) :
		game->KeyboardFunc(c, x, y);
		//gMenu->KeyboardFunc(c, x, y);
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

Mat GameState::readAndResize()
{
	Mat baseFrame;
	Mat resizeMat = Mat(winHeight, winWidth, 16);
	cap.read(baseFrame);
	resize(baseFrame, baseFrame, resizeMat.size());//Resize the captured frame so it fits 1:1 with the screen

	//warpPerspective(baseFrame, baseFrame, transform, baseFrame.size(), INTER_LINEAR, BORDER_CONSTANT);
	return baseFrame;
}

int GameState::getHeight()
{
	return winHeight;
}

void GameState::setHeight(int height)
{
	winHeight = height;
}

int GameState::getWidth()
{
	return winWidth;
}

int GameState::getCamHeight()
{
	return camHeight;
}

int GameState::getCamWidth()
{
	return camWidth;
}

void GameState::setWidth(int width)
{
	winWidth = width;
}

void GameState::setState(int state)
{
	currState = state;
}

VideoCapture GameState::getCap()
{
	return cap;
}

void GameState::setPointTransform(Mat t)
{
	pointTransform = t;
}

void GameState::setCamTransform(Mat t)
{
	camTransform = t;
}

//Essentially a constructor. Moving it out of the constructor though allows 
//other classes (Like Menu, for instance) to use GameState functions,
//such as getHeight/Width.
void GameState::init()
{
	prevFrame = clock();
	winHeight = glutGet(GLUT_SCREEN_HEIGHT);
	winWidth = glutGet(GLUT_SCREEN_WIDTH);

	cap = VideoCapture(1);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	camWidth = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
	camHeight = (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	Point2f dst_vertices[4];
	dst_vertices[0] = Point(0, 0);
	dst_vertices[1] = Point(winWidth, 0);
	dst_vertices[2] = Point(winHeight, winWidth);
	dst_vertices[3] = Point(0, winWidth);


	camTransform = getPerspectiveTransform(dst_vertices, dst_vertices);
	pointTransform = getPerspectiveTransform(dst_vertices, dst_vertices);
	//gMenu = new GameMenu();
	mMenu = new MainMenu();
	game = new Game();
}

Mat GameState::getPointTransform()
{
	return pointTransform;
}

Mat GameState::getCamTransform()
{
	return camTransform;
}

void GameState::setHandle(GLuint handle)
{
	winHandle = handle;
}

float GameState::getScale()
{
	return (currFrame - prevFrame) / 60.f;
}

void GameState::setBottomLeft(Point2f p)
{
	bottomLeft = p;
}

Point2f GameState::getBottomLeft()
{
	return bottomLeft;
}
/*
Game GameState::getGame()
{
	return game;
}*/
/*
GameMenu* GameState::getGMenu()
{
	return gMenu;
}*/

GameState::GameState()
{

}


GameState::~GameState()
{
	delete game;
	delete mMenu;
}
