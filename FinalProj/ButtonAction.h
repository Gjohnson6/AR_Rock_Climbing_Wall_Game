#pragma once
#include "GL\freeglut.h"
#include <iostream>

using namespace std;

static class ButtonActions
{
public:/*
	ButtonActions();
	~ButtonActions();*/

	static void Exit()
	{
		cout << "Exit" << endl;
		glutLeaveMainLoop();
	}
};

