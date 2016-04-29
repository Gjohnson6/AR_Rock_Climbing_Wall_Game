#pragma once
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "glm\glm.hpp"
#include "Game.h"
#include "MainMenu.h"
#include "GameMenu.h"
#include "CameraCalibration.h"
#include <iostream>

using namespace std;

class GameState
{
public:
	void DisplayFunc();
	void MouseFunc(int& button, int& state, int& x, int& y);
	void MouseMotionFunc(int& x, int& y);
	void KeyboardFunc(unsigned char& c, int& x, int& y);
	void SpecialFunc(int key, int x, int y);
	bool GLReturnedError(char * s);

	static GameState* GetInstance()
	{
		if (!instance)
		{
			instance = new GameState;
		}
		return instance;
	}

	Mat readAndResize();
	int getHeight();
	int getWidth();
	int getCamHeight();
	int getCamWidth();
	void setHeight(int height);
	void setWidth(int width);
	void setState(int state);
	VideoCapture getCap();
	void setPointTransform(Mat t);
	void setCamTransform(Mat t);
	void init();
	Mat getPointTransform();
	Mat getCamTransform();
	void setHandle(GLuint handle);
	float getScale();
	void setBottomLeft(Point2f p);
	Point2f getBottomLeft();
	//Game getGame();
	//GameMenu* getGMenu();

	enum GAMESTATE
	{
		MENU,
		GAME,
		CALIB,
		EXIT
	};

private:
	GameState();
	~GameState();
	int winHeight = 1080;
	int winWidth = 1920;
	int camHeight;
	int camWidth;
	int currState = MENU;
	Point2f bottomLeft = (0.f, 0.f);
	clock_t prevFrame;
	clock_t currFrame;
	
	static GameState* instance;

	CameraCalibration calib;
	Game* game = nullptr;
	MainMenu* mMenu = nullptr;
	//GameMenu* gMenu = nullptr;

	VideoCapture cap;
	Mat pointTransform;
	Mat camTransform;

	GLuint winHandle;

};

