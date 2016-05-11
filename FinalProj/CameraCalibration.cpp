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
	//Get the warped and resized frame from the camera
	Mat frame = GameState::GetInstance()->readAndResize();

	warpPerspective(frame, frame, GameState::GetInstance()->getCamTransform(), frame.size(), INTER_LINEAR, BORDER_CONSTANT);

	frame1 = frame;

	Mat image;
	flip(frame, image, 0);

	glRasterPos2i(0, 0);
	glDrawPixels(image.size().width, image.size().height, GL_BGR_EXT, GL_UNSIGNED_BYTE, image.ptr());

	int left = 0, bottom = 0, right = GameState::GetInstance()->getWidth(), top = GameState::GetInstance()->getHeight();
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2i(left, bottom);
	glVertex2i(right, bottom);

	glVertex2i(right, bottom);
	glVertex2i(right, top);

	glVertex2i(right, top);
	glVertex2i(left, top);

	glVertex2i(left, top);
	glVertex2i(left, bottom);

	glEnd();
}

void CameraCalibration::MouseFunc(int & button, int& state, int& x, int& y)
{
	if (state == GLUT_DOWN)
	{
		addBorderPoint(x, y);	
		if (borderPoints.size() == 4)
		{
			TransformImage();//Transform it again
		}
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

void CameraCalibration::TransformImage()
{
	cv::Mat transformedFrame;
	Point2f point_vertices[4];

	point_vertices[0] = borderPoints[0];
	point_vertices[1] = borderPoints[1];
	point_vertices[2] = borderPoints[2];
	point_vertices[3] = borderPoints[3];

	GameState::GetInstance()->setBottomLeft(point_vertices[3]);

	Point2f frame_vertices[4];
	frame_vertices[0] = Point(0, 0);
	frame_vertices[1] = Point(GameState::GetInstance()->getWidth(), 0);
	frame_vertices[2] = Point(GameState::GetInstance()->getWidth(), GameState::GetInstance()->getHeight() - 1);
	frame_vertices[3] = Point(0, GameState::GetInstance()->getHeight());

	Mat pointTransform = getPerspectiveTransform(frame_vertices, point_vertices);
	Mat pointWarp = GameState::GetInstance()->getPointTransform() * pointTransform;
	GameState::GetInstance()->setPointTransform(pointWarp);

	vector<Point2f> points;
	Point2f point = Point2f(2560, 1440);
	points.push_back(point);
	Mat mat = Mat(points, CV_32FC1);
	perspectiveTransform(mat, mat, GameState::GetInstance()->getPointTransform());

	mat.copyTo(points);
	


	Mat camTransform = getPerspectiveTransform(point_vertices, frame_vertices);
	Mat camWarp = GameState::GetInstance()->getCamTransform() * camTransform;
	//camTransform = getPerspectiveTransform(point_vertices, frame_vertices);
	camWarp = GameState::GetInstance()->getCamTransform() * camTransform;
	GameState::GetInstance()->setCamTransform(camWarp);
	//cv::warpPerspective(frame1, transformedFrame, GameState::GetInstance()->getCamTransform(), transformedFrame.size(), INTER_LINEAR, BORDER_CONSTANT);

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
	GameState::GetInstance()->setCamTransform(transform);
	GameState::GetInstance()->setPointTransform(transform);
	borderPoints.clear();	
}