#include "GameMenu.h"
#include "GameState.h"


GameMenu::GameMenu()
{
	addButton(GAME, "Play Game");
	addButton(LOAD, "Load Game");
	addButton(MAIN, "Return");
}

GameMenu::~GameMenu()
{
}

void GameMenu::MouseFunc(int & button, int & mState, int & x, int & y)
{
	clicks++;
	switch (state)
	{
	case(GAME) :
		g.MouseFunc(button, mState, x, y);
		break;
	case(MENU):
		if (mState == GLUT_DOWN)
		{
			switch (DetectClick(x, GameState::GetInstance()->getHeight() - y))
			{
			case(GAME) :
				state = GAME;
				break;
			case(LOAD) :
				state = LOAD;
				break;
			case(MAIN) :
				state = MENU;//Set to MENU so that if we return to this menu, it will display the right menu
				GameState::GetInstance()->setState(GameState::MENU);//Go back to the main menu
				break;
			}
		}
	}
}

void GameMenu::MouseMotionFunc(int& x, int& y)
{
	switch (state)
	{
	case(GAME) :
		g.MouseMotionFunc(x, y);
		break;
	}
}

void GameMenu::DisplayFunc()
{
	switch (state)
	{
	case(MENU):
		for (auto button : buttons)
		{
			button.Draw();
		}
		break;
	case(GAME) :
		g.DisplayFunc();
		break;
	case(LOAD) :

		break;
	}
}

void GameMenu::KeyboardFunc(unsigned char& c, int& x, int& y)
{
	switch (state)
	{
	case(GAME) :
		g.KeyboardFunc(c, x, y);
		break;
	}
}

void GameMenu::setState(int s)
{
	state = s;
}
