#include "Game.h"

Game::Game(int width, int height)
{
	this->width = width;
	this->height = height;
	capture = VideoCapture(1);
}


Game::~Game()
{
}

void Game::init()
{
}

int Game::AddMarker(int x, int y)
{
	int retVal = -1;
	if (this->markers.size() < 9)
	{
		HitMarker mark = HitMarker(this->markers.size() + 1, x,  y);
		this->markers.push_back(mark);
		retVal = mark.getNum() - 1;
	}

	return retVal;
}


void Game::DetectHit(Mat thresholdImage)
{
	if (currToHit < markers.size())
	{
		Scalar colour = thresholdImage.at<uchar>(Point(markers[currToHit].getX(), thresholdImage.size().height - markers[currToHit].getY()));
		if (colour.val[0] == 255)
		{
			if (currToHit == 0)
			{
				startTime = clock();
			}
			markers[currToHit].getHit();
			currToHit++;
			if (currToHit == markers.size())
			{
				currTime = clock() - startTime;
				finished = true;
			}
		}
	}
	if (currToHit != 0 && !finished)
	{
		currTime = clock() - startTime;
	}
}

void Game::Reset()
{
	for (int i = 0; i < markers.size(); i++)
	{
		markers[i].Reset();
		startTime = 0;
		currTime = 0;
	}
	finished = false;
	currToHit = 0;
}

void Game::DisplayTimer()
{
	glPushMatrix();
	if (finished)
	{
		glColor3d(0.0, 1.0, 0.0);
	}
	else
	{
		glColor3d(1.0, 1.0, 1.0);
	}
	glScaled(.2, .2, .2);

	stringstream strTime;
	strTime << setprecision(2) << to_string((float)currTime / CLOCKS_PER_SEC);
	//string strTime = to_string((float)currTime / CLOCKS_PER_SEC);
	//glutStrokeString(GLUT_ _MONO_ROMAN, (const unsigned char *)strTime.str().c_str());
	glRasterPos2i(4, 4);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *)strTime.str().c_str());
	glPopMatrix();
}

//When deleting markers, change the markers' numbers to correspond
void Game::DeleteMarker(int x, int y)
{
	cout << "DELETE" << endl;
	for (int i = 0; i < markers.size(); i++)
	{
		if (markers[i].DetectMouse(x, y))
		{
			markers.erase(markers.begin() + i);
			for (int i = 0; i < markers.size(); i++)
			{
				markers[i].setNum(i + 1);
			}
			break;
		}
	}
}

void Game::DisplayFunc()
{
		capture.read(frame1);
		//convert frame2 to gray scale for frame differencing
		cv::cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);		
		


		//GaussianBlur(grayImage1, grayImage1, Size(9, 9), 2, 2);

		/*
		vector<Vec3f> circles;
		cv::HoughCircles(grayImage1, circles, CV_HOUGH_GRADIENT, 1, grayImage1.rows / 8, 200, 50, 0, 0);


		Mat circlesmat;
		for (size_t i = 0; i < circles.size(); i++)
		{
			cout << "Circle detected" << endl;
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);

			circle(frame1, center, 3, Scalar(0, 255, 0), -1, 8, 0);

			circle(frame1, center, radius, Scalar(0, 0, 255), 3, 8, 0);
		}

		if (circles.size() > 0)
		{
			//namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
			cv::imshow("Circles", frame1);
		}


		cv::HoughCircles(grayImage1, circles, CV_HOUGH_GRADIENT, 1, 30, 100, 45, 0, 0);
		
		for (size_t i = 0; i < circles.size(); i++)
		{
			cout << "Circle detected" << endl;
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);

			circle(frame1, center, 3, Scalar(0, 255, 0), -1, 8, 0);

			circle(frame1, center, radius, Scalar(0, 0, 255), 3, 8, 0);
		}

		if (circles.size() > 0)
		{
			//namedWindow("Hough Circle Transform Demo 2", CV_WINDOW_AUTOSIZE);
			cv::imshow("Circles 2", frame1);
		}
		*/
		bool wasFound;

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
		
		
		
		
		
	//if (!calibrated)
	//{
	//	if (flip)
	//		glColor3d(1, 1, 1);
	//	else
	//		glColor3d(0, 0, 0);
	//	flip = !flip;
	//	glBegin(GL_LINES);

	//	for (int i = 0; i < width; i++)
	//	{
	//		if (i % 72 == 0)
	//		{
	//			glVertex2i(i, 0);
	//			glVertex2i(i, height);
	//		}
	//	}

	//	for (int i = 0; i < 1280; i++)
	//	{
	//		if (i % 128 == 0)
	//		{
	//			glVertex2i(0, i);
	//			glVertex2i(width, i);
	//		}
	//	}

	//	glEnd();
	//}

//	{


		
		



		GLReturnedError("Entering Display Func");
		//glClearColor(51 / 255.0f, 51 / 255.0f, 51 / 255.0f, 1.0f);

		DetectHit(thresholdImage);
		DisplayTimer();
		for (int i = 0; i < markers.size(); i++)
		{
			markers[i].Draw();
		}
		
	

//	}	
	if (debugMode == true) {
			//show the difference image and threshold image
			cv::imshow("Difference Image", differenceImage);
			cv::imshow("Threshold Image", thresholdImage);
			cv::imshow("Normal Image", frame1);
			cv::imshow("Gray image 1", grayImage1);
			
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
}
