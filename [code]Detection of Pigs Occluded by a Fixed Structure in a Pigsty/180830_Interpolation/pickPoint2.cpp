#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>  
#include <algorithm>

using namespace cv;
using namespace std;



int main()
{

	cv::VideoCapture Video("target_origin_1.avi");//video파일을 불러와서 
	if (!Video.isOpened())//찾지 못하였을 경우 
	{
		return 0; //종료
	}

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	cv::Mat col1_1 = imread("D:\\img_25.jpg", 0);

	cv::Mat origin, dst, dst1;
	cv::Mat Grayscale1, Grayscale;
	vector<vector<Point> >hull(contours.size());

	while (true) {


		Video >> origin; //video에서 프레임을 받아와 넘겨준다


		cvtColor(origin, Grayscale, CV_BGR2GRAY);

		Canny(Grayscale(cv::Rect(183, 167, 43, 25)), dst1, 30, 60, 3);

		findContours(dst1, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(183, 167));

		drawContours(Grayscale, contours, -1, (0, 0, 80), 1);



		for (int i = 0; i < contours.size(); i++) {

			for (int j = 0; j < contours[i].size(); j++)
			{
				//convexHull(Mat(contours[i][j]), hull[i][j], false);
			}

			for (int j = 0; j < contours[i].size(); j++) {

				//drawContours(Grayscale, hull, i, (0, 0, 80), 1, 8, vector<Vec4i>(), 0, Point());

			}
		}


		imshow("dst221", Grayscale);


		if (cv::waitKey(0) == 27) {
			cv::destroyWindow("video"); break;//ESC를 누르면 종료
		}

	}

	return 0;
}
