#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\calib3d.hpp"
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "glm\glm.hpp"
#include <vector>

using namespace std;
using namespace cv;

class CameraCalibration
{
public:
	CameraCalibration();
	~CameraCalibration();

	void DisplayFunc();
	void MouseFunc(int&, int&, int&, int&);
	void KeyboardFunc(unsigned char&, int&, int&);
	void SpecialFunc(int key, int x, int y);
	void addBorderPoint(int x, int y);
private:

	void TransformImage();
	void ResetTransform();
	cv::Mat frame1;

	vector<cv::Point> borderPoints;
};

