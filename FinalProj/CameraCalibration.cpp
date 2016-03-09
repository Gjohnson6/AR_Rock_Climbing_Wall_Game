#include "CameraCalibration.h"



CameraCalibration::CameraCalibration()
{
	capture = VideoCapture(1);
}


CameraCalibration::~CameraCalibration()
{
}

void CameraCalibration::DisplayFunc()
{
	capture.read(frame1);
	//convert frame2 to gray scale for frame differencing
	cv::cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);

	std::vector<std::vector<cv::Point2f> > imagePoints(1);
	cv::Size boardSize(7, 7);
	//copy second frame
	capture.read(frame2);
	//convert frame2 to gray scale for frame differencing
	cv::cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);
	//perform frame differencing with the sequential images. This will output an "intensity image"
	//do not confuse this with a threshold image, we will need to perform thresholding afterwards.
	cv::absdiff(grayImage1, grayImage2, differenceImage);
	//threshold intensity image at a given sensitivity value
	cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

	GLReturnedError("Entering Display Func");

	if (borderPoints.size() == 4)
	{
		TransformImage();
	}
}

void CameraCalibration::addBorderPoint(int x, int y)
{
	borderPoints.push_back(Point(x, y));
}

cv::Mat CameraCalibration::TransformImage()
{
	cv::Mat transformedFrame;


	Point2f src_vertices[4];

	src_vertices[0] = borderPoints[0];
	src_vertices[1] = borderPoints[1];
	src_vertices[2] = borderPoints[2];
	src_vertices[3] = borderPoints[3];


	Point2f dst_vertices[4];
	dst_vertices[0] = Point(0, 0);
	dst_vertices[1] = Point(1280, 0);
	dst_vertices[2] = Point(1280, 720);
	dst_vertices[3] = Point(0, 720);

	Mat transform = getPerspectiveTransform(src_vertices, dst_vertices);

	cv::warpPerspective(frame1, transformedFrame, transform, transformedFrame.size(), INTER_LINEAR, BORDER_CONSTANT);

	cv::imshow("Transformed", transformedFrame);
	return transformedFrame;
}
