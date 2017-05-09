#include "RightSideMenu.h"
#include "GameState.h"

void RightSideMenu::addButton(int menuNum, string name)
{
	int left = (int)(GameState::GetInstance()->getWidth() * .75f * .25f + GameState::GetInstance()->getWidth() * .25f);
	int right = (int)(GameState::GetInstance()->getWidth() * .75f * .75f + GameState::GetInstance()->getWidth() * .25f);
	
	int top = GameState::GetInstance()->getHeight() - 40;
	int bottom = GameState::GetInstance()->getHeight() - 80;

	//Because the loadmenu will have pages, the shifting is going to work differently. 
	//MenuNum will define the distance from the top of the screen. So if multiple buttons have the same menunum,
	//They'll be at the same height (but they'll be on different pages)
	int pos = (buttons.size() % maxButtons);// -1;
	if (pos == -1)
	{
		pos = maxButtons - 1;
	}
	int shift = 60 * pos;
	top -= shift;
	bottom -= shift;
	if ((buttons.size() + 1) % maxButtons == 0)
	{
		buttons.push_back(Button(left, right, top, bottom, buttons.size() % maxButtons, "Next Page"));
		buttons.push_back(Button(left, right, GameState::GetInstance()->getHeight() - 40, GameState::GetInstance()->getHeight() - 80, buttons.size() % maxButtons, "Previous Page"));
		pos = (buttons.size() % maxButtons);
		shift = 60 * pos;
		top = GameState::GetInstance()->getHeight() - 40;
		bottom = GameState::GetInstance()->getHeight() - 80;
		top -= shift;
		bottom -= shift;
		buttons.push_back(Button(left, right, top, bottom, buttons.size() % maxButtons, name));
	}
	else
	{
		buttons.push_back(Button(left, right, top, bottom, buttons.size() % maxButtons, name));
	}
}

void RightSideMenu::DisplayFunc()
{
	for (unsigned int i = page * maxButtons; i < (page + 1) * maxButtons && i < buttons.size(); i++)
	{
		buttons[i].Draw();
	}
}

void RightSideMenu::PageRight()
{
	if (page * maxButtons + maxButtons < buttons.size())
	{
		page++;
	}
}

void RightSideMenu::PageLeft()
{
	if (page > 0)
	{
		page--;
	}
}

void RightSideMenu::Clear()
{
	buttons.clear();
}

int RightSideMenu::getMaxButtons()
{
	return maxButtons;
}

//Instead of returning menu num, we're going to return the name of the button. 
string RightSideMenu::detectClickReturnString(int x, int y)
{
	string retVal = "";
	for (unsigned int i = page * maxButtons; i < (page + 1) * maxButtons && i < buttons.size(); i++)
	{
		if (buttons[i].DetectClick(x, y) >= 0)
		{
			retVal = buttons[i].GetName();
			break;
		}
	}

	return retVal;
}

vector<Button> RightSideMenu::getButtons()
{
	return buttons;
}

RightSideMenu::RightSideMenu()
{
	maxButtons = (GameState::GetInstance()->getHeight() - 20) / 60;
}


RightSideMenu::~RightSideMenu()
{
}