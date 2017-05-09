#include "GamePlayMenu.h"
#include "GameState.h"

void GamePlayMenu::addButton(int menuNum, string name)
{	
	//The left and right bounds will be easy to figure out because all the buttons are the same width.
	int left = (GameState::GetInstance()->getWidth() / 4) / 4;
	int right = ((GameState::GetInstance()->getWidth() / 4) * 3) / 4;

	//The bottom and top bounds are harder, because they'll change as new buttons are added.
	//First we start with the basic top and bottom bounds
	int top = 80;
	int bottom = 40;

	//Contrary to Menu, the buttons will have to be added bottom to top because of the way shift works.
	//It wouldn't be difficult to make it so that the button's positions change as more are added but this is simple and easy to understand.
	int shift = 60 * buttons.size();
	top += shift;
	bottom += shift;
	
	buttons.push_back(Button(left, right, top, bottom, menuNum, name));
}

GamePlayMenu::GamePlayMenu()
{

}


GamePlayMenu::~GamePlayMenu()
{
}