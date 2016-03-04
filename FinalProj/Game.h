#pragma once
#include "HitMarker.h"
#include <vector>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sys/timeb.h>
#include <time.h>
#include <iomanip>

using namespace std;
using namespace cv;

class Game
{
public:
	Game(int width = 1280, int height = 720);
	~Game();

	void init();
	int AddMarker(int x, int y);
	void DetectHit(cv::Mat threshold);
	void Reset();
	void DeleteMarker(int x, int y);
	void(*DisplayFunc)();
	void(*RedisplayFunc)(int, int);
	void(*MouseFunction)(int, int, int, int);
	void DisplayTimer();

	GLuint handle;
	int width;
	int height;
	vector<HitMarker> markers;
private:
	int currToHit = 0;
	clock_t startTime = 0;
	clock_t currTime;
	bool finished = false;
	enum State {
		Menu,
		New,
		Play,
		Results
	};
};

