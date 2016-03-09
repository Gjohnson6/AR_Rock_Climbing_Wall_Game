#include "MainMenu.h"



MainMenu::MainMenu()
{
	addButton("Play");
	addButton("Calibrate Camera");
	addButton("Exit");
}


MainMenu::~MainMenu()
{
}

void MainMenu::Exit()
{
	glutLeaveMainLoop();
}