#include "Menu.h"



Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::DisplayFunc()
{

	for (auto button : buttons)
	{
		button.Draw();
	}
}

void Menu::DetectClick(int x, int y)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		if(buttons[i].DetectClick(x, y))
			cout << "Menu clicked" << endl;
	}
}


void Menu::addButton(string name)
{
	//The left and right bounds will be easy to figure out because all the buttons are the same width.
	int left = 440;
	int right = 840;

	//The bottom and top bounds are harder, because they'll change as new buttons are added.
	//First we start with the basic top and bottom bounds
	int top = 720 - 40;
	int bottom = 720 - 80;

	//Then for each button on the screen already, we subtract 60 from both, to move the button down and add some spacing between the buttons
	int shift = 60 * buttons.size();
	top -= shift;
	bottom -= shift;

	buttons.push_back(Button(left, right, top, bottom, name));
}
