#pragma once
#include "GamePlayMenu.h"
class InGameMenu : GamePlayMenu
{
public:
	InGameMenu();
	~InGameMenu();
private:
	enum ButtonNums
	{
		READY,
		RESET,
		RETURN
	};
};

