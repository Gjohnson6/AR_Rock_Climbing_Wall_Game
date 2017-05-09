#pragma once
#include "Menu.h"

//This is a menu that is displayed on the left while the game is being played.
class GamePlayMenu : public Menu
{

public:
	void addButton(int menuNum, string name);
	GamePlayMenu();
	~GamePlayMenu();
};