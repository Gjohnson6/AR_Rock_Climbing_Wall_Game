#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\calib3d.hpp"

#include "HitMarker.h"
#include <vector>
#include <sys/timeb.h>
#include <time.h>
#include <iomanip>

using namespace std;
using namespace cv;

class Game
{
public:
	Game();
	~Game();

	int AddMarker(int x, int y);//Adds a marker to the map
	void DetectHit(cv::Mat threshold);//Checks if the current marker was hit
	void Reset();//Resets the map so all markers are not hit and the clock is stopped at 0
	void DeleteMarker(int x, int y);//Attempts to remove a marker at a given point.
	
	void DisplayFunc();
	void MouseFunc(int & button, int & state, int & x, int & y);
	void MouseMotionFunc(int& x, int& y);
	void KeyboardFunc(unsigned char & c, int & x, int & y);
	//Displays the timer in the bottom left corner. When the map is finished, it turns green.
	void DisplayTimer();

	GLuint handle;
	int width;
	int height;
	vector<HitMarker> markers;
	bool debugMode = false;
private:
	int currToHit = 0;//Index of the current hittable hitmarker in the vector. This is used to make sure markers can't be hit out of order
	int currMarker = -1;//Used for moving markers
	clock_t startTime = 0;//The time the fist marker is hit
	clock_t currTime = 0;//The current time
	bool finished = false;//Whether the last marker has been hit
	bool mouseHeld = false;

	Mat currFrame;
	Mat currGray, prevGray;
	Mat differenceImage;
	Mat thresholdImage;
	const static int SENSITIVITY_VALUE = 20;
	const static int BLUR_SIZE = 30;

	bool calibrated = false;
	bool flip = false;
	bool thresh = false;
	bool diff = false;
};

