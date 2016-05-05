#include "GameState.h"
#include <iostream>

using namespace std;
using namespace glm;
using namespace cv;

GameState* GameState::instance = 0;

void MouseFunc(int button, int state, int x, int y)
{
	GameState::GetInstance()->MouseFunc(button, state, x, y);
}

void MouseMotionFunc(int x, int y)
{
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
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(GameState::GetInstance()->getWidth(), GameState::GetInstance()->getHeight());
	GameState::GetInstance()->setHandle(glutCreateWindow("Game"));
	glutDisplayFunc(GameDisplayFunc);
	glutMouseFunc(MouseFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutTimerFunc(1000 / 30, TimerFunc, 1000 / 30 );
	glutMotionFunc(MouseMotionFunc);
	glutFullScreen();
	glLineWidth(2.15);
	glEnable(GL_LINE_SMOOTH);
	glutMainLoop();
}