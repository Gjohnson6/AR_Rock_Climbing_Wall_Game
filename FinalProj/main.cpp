#include "GameState.h"
#include <iostream>

using namespace std;
using namespace glm;
using namespace cv;

GameState* GameState::instance = 0;
//Game window;
int currMarker;
int currToHit = 0;
bool debugMode = false;
bool mouseHeld = false;
const static int SENSITIVITY_VALUE = 20;


MainMenu menu;

const static int BLUR_SIZE = 30;

void ReshapeFunc(int width, int height)
{
	//window.width = width;
	//window.height = height;
}

void MouseFunc(int button, int state, int x, int y)
{
	GameState::GetInstance()->MouseFunc(button, state, x, y);
	/*
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			cameraCal.addBorderPoint(x, y);
			bool dragging = false;
			for (auto mark : window.markers)
			{
				if (mark.DetectMouse(x, window.height - y))
				{
					currMarker = mark.getNum() - 1;
					dragging = true;
				}
			}
			if (!dragging)
			{
				currMarker = window.AddMarker(x, window.height - y);
			}
			mouseHeld = true;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			window.DeleteMarker(x, window.height - y);
		}
	}
	else if (state == GLUT_UP)
	{
		mouseHeld = false;
	}

	menu.DetectClick(x, window.height - y);
	*/
}

void MouseMotionFunc(int x, int y)
{
	/*if (mouseHeld && currMarker >= 0)
	{
		window.markers[currMarker].move(x, window.height - y);
	}*/
	GameState::GetInstance()->MouseMotionFunc(x, y);
}

void GameDisplayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, GameState::GetInstance()->getWidth(), 0, GameState::GetInstance()->getHeight(), -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glViewport(0, 0, GameState::GetInstance()->getWidth(), GameState::GetInstance()->getHeight());

	GameState::GetInstance()->DisplayFunc();
	glutSwapBuffers();
}

void TimerFunc(int period)
{
	glutTimerFunc(period, TimerFunc, period);
	glutPostRedisplay();
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	GameState::GetInstance()->KeyboardFunc(c, x, y);
}

void SpecialFunc(int key, int x, int y)
{
	GameState::GetInstance()->SpecialFunc(key, x, y);
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	GameState::GetInstance()->init();

	//This is a fullscreen application, so we set the width and height to the size of the screen.

	//window.DisplayFunc = GameDisplayFunc;
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(GameState::GetInstance()->getWidth(), GameState::GetInstance()->getHeight());
	GameState::GetInstance()->setHandle(glutCreateWindow("Name"));
	glutReshapeFunc(ReshapeFunc);
	glutDisplayFunc(GameDisplayFunc);
	//glutDisplayFunc(MenuDisplayFunc);
	glutMouseFunc(MouseFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutTimerFunc(1000 / 30, TimerFunc, 1000 / 30 );
	glutMotionFunc(MouseMotionFunc);
	glutFullScreen();
	glutMainLoop();
}