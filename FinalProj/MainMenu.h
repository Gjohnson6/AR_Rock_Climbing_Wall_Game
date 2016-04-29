#pragma once
#include "Menu.h"
#include "GamePlayMenu.h"

class MainMenu : public Menu
{
public:
	MainMenu();
	~MainMenu();

	void MouseFunc(int& button, int& state, int& x, int& y);
private:
	enum {
		PLAY,
		CALIB,
		EXIT
	};

	typedef Menu super;

};

