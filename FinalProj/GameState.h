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

	int getHeight();
	int getWidth();
	void setState(int state);
	VideoCapture getCap();
	void setTransform(Mat t);
	Mat getTransform();

	Game getGame();
	GameMenu* getGMenu();

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
	int currState = MENU;
	static GameState* instance;

	CameraCalibration calib;
	Game game;
	MainMenu mMenu;
	GameMenu* gMenu;

	VideoCapture cap;
	Mat transform;

};

