#pragma once
#include "Menu.h"

class RightSideMenu : public Menu
{
public:
	void addButton(int menuNum, string name);
	void DisplayFunc();
	void PageRight();
	void PageLeft();
	void Clear();
	int getMaxButtons();
	string detectClickReturnString(int x, int y);
	vector<Button> getButtons();
	
	RightSideMenu();
	~RightSideMenu();
private:
	unsigned int page = 0;
	unsigned int maxButtons;
};