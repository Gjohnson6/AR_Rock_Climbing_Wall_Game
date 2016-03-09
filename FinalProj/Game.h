#pragma once
#include "HitMarker.h"
#include "GameState.h"
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\calib3d.hpp"
#include <sys/timeb.h>
#include <time.h>
#include <iomanip>

using namespace std;
using namespace cv;

class Game : public GameState
{
public:
	Game(int width = 1280, int height = 720);
	~Game();

	void init();
	int AddMarker(int x, int y);//Adds a marker to the map
	void DetectHit(cv::Mat threshold);//Checks if the current marker was hit
	void Reset();//Resets the map so all markers are not hit and the clock is stopped at 0
	void DeleteMarker(int x, int y);//Attempts to remove a marker at a given point.
	
	//Not used at the moment
	void DisplayFunc();
	void(*RedisplayFunc)(int, int);
	void(*MouseFunction)(int, int, int, int);

	//Displays the timer in the bottom left corner. When the map is finished, it turns green.
	void DisplayTimer();

	GLuint handle;
	int width;
	int height;
	vector<HitMarker> markers;
	bool debugMode = false;
private:
	int currToHit = 0;//Index of the current hittable hitmarker in the vector. This is used to make sure markers can't be hit out of order
	clock_t startTime = 0;//The time the fist marker is hit
	clock_t currTime;//The current time
	bool finished = false;//Whether the last marker has been hit

	VideoCapture capture;
	Mat frame1, frame2;
	Mat grayImage1, grayImage2;
	Mat differenceImage;
	Mat thresholdImage;
	const static int SENSITIVITY_VALUE = 20;
	const static int BLUR_SIZE = 30;

	bool calibrated = false;
	bool flip = false;
	enum State {//Enum that might be used to for different screens. 
		Menu,
		New,
		Play,
		Results
	};
};

