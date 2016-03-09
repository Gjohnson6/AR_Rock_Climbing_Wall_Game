#pragma once
#include "Game.h"

class CameraCalibration : public GameState
{
public:
	CameraCalibration();
	~CameraCalibration();


	void DisplayFunc();

	void addBorderPoint(int x, int y);

private:

	cv::Mat TransformImage();
	VideoCapture capture;
	Mat frame1, frame2;
	Mat grayImage1, grayImage2;
	Mat differenceImage;
	Mat thresholdImage;
	const static int SENSITIVITY_VALUE = 20;
	const static int BLUR_SIZE = 30;

	vector<cv::Point> borderPoints;
};

