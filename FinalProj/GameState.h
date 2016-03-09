#pragma once
#include "GL\freeglut.h"
#include <iostream>

using namespace std;

class GameState
{
public:
	virtual void DisplayFunc() = 0;
	bool GLReturnedError(char * s);

	GameState();
	~GameState();
};

