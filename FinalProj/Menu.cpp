#include "Menu.h"
#include "GameState.h"
Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::DisplayFunc()
{
	//glRasterPos2i(0, 0);
	//glDrawPixels(bg.size().width, bg.size().height, GL_RGB, GL_UNSIGNED_BYTE, bg.ptr());
	for (auto button : buttons)
	{
		button.Draw();
	}
}

int Menu::DetectClick(int x, int y)
{
	int retVal = -1;
	for (int i = 0; i < buttons.size(); i++)
	{
		if ((retVal = buttons[i].DetectClick(x, y)) >= 0)
		{	
			cout << i << endl;
			break;
		}
	}

	return retVal;
}


void Menu::addButton(int menuNum, string name)
{
	//The left and right bounds will be easy to figure out because all the buttons are the same width.
	int left = 440;
	int right = 1480;

	//The bottom and top bounds are harder, because they'll change as new buttons are added.
	//First we start with the basic top and bottom bounds
	int top = 1080 - 40;
	int bottom = 1080 - 80;

	//Then for each button on the screen already, we subtract 60 from both, to move the button down and add some spacing between the buttons
	int shift = 60 * buttons.size();
	top -= shift;
	bottom -= shift;

	buttons.push_back(Button(left, right, top, bottom, menuNum, name));
}
