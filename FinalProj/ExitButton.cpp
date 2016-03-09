#include "ExitButton.h"


ExitButton::~ExitButton()
{
}

void ExitButton::Do()
{
	glutLeaveMainLoop();
}
