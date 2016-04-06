#include "MainMenu.h"
#include "GameState.h"
MainMenu::MainMenu()
{
	addButton(PLAY, "Play");
	addButton(CALIB, "Calibrate Camera");
	addButton(EXIT, "Exit");
}

MainMenu::~MainMenu()
{
}

void MainMenu::MouseFunc(int & button, int & state, int & x, int & y)
{
	if (state == GLUT_DOWN)
	{
		switch (DetectClick(x, GameState::GetInstance()->getHeight() - y))
		{
		case(PLAY):
			GameState::GetInstance()->setState(GameState::GAME);
			break;
		case(CALIB):
			GameState::GetInstance()->setState(GameState::CALIB);
			break;
		case(EXIT) :
			GameState::GetInstance()->setState(GameState::EXIT);
			break;
		}
	}
}