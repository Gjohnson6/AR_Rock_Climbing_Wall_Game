#pragma once
#include "Button.h"
#include "GL\freeglut.h"

class ExitButton : public Button
{
public:
	ExitButton();
	~ExitButton();

	void Do();
};

