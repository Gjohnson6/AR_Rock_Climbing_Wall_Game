#pragma once
#include "Button.h"
#include "GameState.h"
#include <vector>
#include <iostream>

using namespace std;

class Menu : public GameState
{
public:
	Menu();
	~Menu();

	void DisplayFunc();
	void DetectClick(int x, int y);
protected:
	vector<Button> buttons;
	void addButton(string name);
};

