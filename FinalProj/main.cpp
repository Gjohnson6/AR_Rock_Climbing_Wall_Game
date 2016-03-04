#include "Game.h"
#include <iostream>
#include "glm\glm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace glm;
using namespace cv;

Game window;
VideoCapture capture(0);
Mat frame1, frame2;
Mat grayImage1, grayImage2;
Mat differenceImage;
Mat thresholdImage;
int currMarker;
int currToHit = 0;
bool debugMode = false;
bool mouseHeld = false;

const static int SENSITIVITY_VALUE = 20;

const static int BLUR_SIZE = 30;


bool GLReturnedError(char * s)
{
	bool return_error = false;
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		cerr << s << ": " << gluErrorString(glerror) << endl;
	}

	return return_error;
}

void ReshapeFunc(int width, int height)
{
	window.width = width;
	window.height = height;
}

void MouseFunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
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

}

void MousePassivefunc(int x, int y)
{
	for (int i = 0; i < window.markers.size(); i++)
	{
		if (window.markers[i].DetectMouse(x, window.height - y))
		{
			//window.markers[i].getHit();
		}
	}
}

void MouseMotionFunc(int x, int y)
{
	if (mouseHeld && currMarker >= 0)
	{
		window.markers[currMarker].move(x, window.height - y);
	}
}

void DisplayFunc()
{
	capture.read(frame1);
	//convert frame2 to gray scale for frame differencing
	cv::cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);
	//copy second frame
	capture.read(frame2);
	//convert frame2 to gray scale for frame differencing
	cv::cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);
	//perform frame differencing with the sequential images. This will output an "intensity image"
	//do not confuse this with a threshold image, we will need to perform thresholding afterwards.
	cv::absdiff(grayImage1, grayImage2, differenceImage);
	//threshold intensity image at a given sensitivity value
	cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
	if (debugMode == true) {
		//show the difference image and threshold image
		cv::imshow("Difference Image", differenceImage);
		cv::imshow("Threshold Image", thresholdImage);
		cv::imshow("Normal Image", frame1);

		//blur the image to get rid of the noise. This will output an intensity image
		cv::blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
		//threshold again to obtain binary image from blur output
		cv::threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

		//show the threshold image after it's been "blurred"
		imshow("Final Threshold Image", thresholdImage);
		
	}
	else {
		//if not in debug mode, destroy the windows so we don't see them anymore
		cv::destroyWindow("Difference Image");
		cv::destroyWindow("Threshold Image");
	}

	

	//GLReturnedError("Entering Display Func");
	//glClearColor(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, window.width, 0, window.height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);
	glViewport(0, 0, window.width, window.height);
	
	window.DetectHit(thresholdImage);
	window.DisplayTimer();
	for (int i = 0; i < window.markers.size(); i ++)
	{
		window.markers[i].Draw();
	}

	glutSwapBuffers();
}

void TimerFunc(int period)
{
	glutTimerFunc(period, TimerFunc, period);
	glutPostRedisplay();
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'd':
		debugMode = !debugMode;
		break;
	case '4':
		window.Reset();
		break;
	}
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	Game window;
	capture.set(CV_CAP_PROP_FRAME_WIDTH, window.width);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, window.height);
	window.DisplayFunc = DisplayFunc;
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window.width, window.height);
	window.handle = glutCreateWindow("Name");
	glutReshapeFunc(ReshapeFunc);
	glutDisplayFunc(window.DisplayFunc);
	glutMouseFunc(MouseFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutTimerFunc(1000 / 144, TimerFunc, 1000 / 144);
	glutMotionFunc(MouseMotionFunc);
	glutPassiveMotionFunc(MousePassivefunc);
	glutMainLoop();
}