#include "CameraCalibration.h"
#include "GameState.h"

CameraCalibration::CameraCalibration()
{
}


CameraCalibration::~CameraCalibration()
{
}

void CameraCalibration::DisplayFunc()
{
	//Read a frame from the camera
	GameState::GetInstance()->getCap().read(frame1);
	std::vector<std::vector<cv::Point2f> > imagePoints(1);
	warpPerspective(frame1, frame1, GameState::GetInstance()->getTransform(), frame1.size(), INTER_LINEAR, BORDER_CONSTANT);
	if (borderPoints.size() == 4)
	{
		TransformImage();
	}
	Mat image;
	flip(frame1, image, 0);
	glDrawPixels(image.size().width, image.size().height, GL_BGR_EXT, GL_UNSIGNED_BYTE, image.ptr());

}

void CameraCalibration::MouseFunc(int & button, int& state, int& x, int& y)
{
	if (state == GLUT_DOWN)
	{
		addBorderPoint(x, y);
	}
}

void CameraCalibration::KeyboardFunc(unsigned char & c, int & x, int & y)
{
	switch (c)
	{
	case 'r'://reset
		ResetTransform();
		break;
	case 27://esc, return to main menu
		GameState::GetInstance()->setState(GameState::MENU);
		break;
	}
}

void CameraCalibration::SpecialFunc(int key, int x, int y)
{

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
	dst_vertices[1] = Point(1920, 0);
	dst_vertices[2] = Point(1920, 1080);
	dst_vertices[3] = Point(0, 1080);

	Mat transform = getPerspectiveTransform(src_vertices, dst_vertices);
	GameState::GetInstance()->setTransform(transform);
	cv::warpPerspective(frame1, transformedFrame, transform, transformedFrame.size(), INTER_LINEAR, BORDER_CONSTANT);

	return transformedFrame;
}

void CameraCalibration::ResetTransform()
{
	int height = GameState::GetInstance()->getHeight();
	int width = GameState::GetInstance()->getWidth();
	Point2f dst_vertices[4];
	dst_vertices[0] = Point(0, 0);
	dst_vertices[1] = Point(width, 0);
	dst_vertices[2] = Point(height, width);
	dst_vertices[3] = Point(0, width);

	Mat transform = getPerspectiveTransform(dst_vertices, dst_vertices);
	GameState::GetInstance()->setTransform(transform);
	borderPoints.clear();	
}