#pragma once
#include "Menu.h"
#include "Game.h"

class GameMenu : public Menu
{
public:
	GameMenu();
	~GameMenu();

	void MouseFunc(int& button, int& state, int& x, int& y);
	void MouseMotionFunc(int & x, int & y);
	void DisplayFunc();
	void KeyboardFunc(unsigned char & c, int & x, int & y);
	void setState(int s);

	enum {
		MENU, 
		GAME,
		LOAD,
		MAIN
	};

private:
	int state = 0;
	int clicks = 0;
	Game g;
};

