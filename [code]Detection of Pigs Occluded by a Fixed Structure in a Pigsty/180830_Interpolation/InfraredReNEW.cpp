#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>  
#include <Windows.h>
using namespace cv;
using namespace std;


#define sample_size 3

int main()
{

	cv::VideoCapture Video("D:\\보간 탐지 영상\\적외선\\Intpl_2018-08-18-10-09-05-Infrared_Video_63.avi");//video파일을 불러와서 
	if (!Video.isOpened())//찾지 못하였을 경우 
	{
		return 0; //종료
	}
	cv::Mat clean_1 = imread("img_134.bmp", 0);
	cv::Mat clean_2 = imread("img_9.bmp", 0);

	cv::Mat origin, dst;
	cv::Mat Grayscale;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Video >> origin;
	int fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D');

	VideoWriter *vp2 = new VideoWriter;

	if (!vp2->open("Intpl_2018-08-18-10-09-05-Infrared_Video_63.avi", fourcc, 10, origin.size(), false)) {
		delete vp2;
		return 0;
	}

	double avgTime = 0;
	int count = 0;



	while (true) {
		LARGE_INTEGER freq, start, end;

		Video >> origin; //video에서 프레임을 받아와 넘겨준다

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start);

		cvtColor(origin, origin, CV_BGR2GRAY);


		//imshow("origin", origin);

		cv::Mat RoiImage1;
		cv::Mat RoiImage2;

		RoiImage1 = origin(cv::Rect(182, 107, 52, 14));
		RoiImage2 = origin(cv::Rect(344, 95, 23, 24));

		clean_1.copyTo(RoiImage1, clean_1);
		clean_2.copyTo(RoiImage2, clean_2);

/*
		Mat temp2;
		resize(origin(cv::Rect(341, 92, 30, 30)), temp2, Size(30 / 2, 30 / 2), INTER_LINEAR);
		resize(temp2, temp2, Size(30, 30), INTER_LINEAR);
		resize(temp2, temp2, Size(30 / 2, 30 / 2), INTER_LANCZOS4);
		resize(temp2, temp2, Size(30, 30), INTER_LANCZOS4);
		temp2.copyTo(origin(cv::Rect(341, 92, 30, 30)), temp2);

		Mat temp;
		resize(origin(cv::Rect(179, 104, 58, 20)), temp, Size(58 / 2, 20 / 2), INTER_LINEAR);
		resize(temp, temp, Size(58, 20), INTER_LINEAR);
		resize(temp, temp, Size(58 / 2, 20 / 2), INTER_LANCZOS4);
		resize(temp, temp, Size(58, 20), INTER_LANCZOS4);
		temp.copyTo(origin(cv::Rect(179, 104, 58, 20)), temp);
*/

		Canny(origin(cv::Rect(182, 107, 52, 14)), dst, 60, 110, 3);
		findContours(dst, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(182, 107));

		for (int i = 0; i < contours.size(); i++) {

			Point standard, point1, point2;

			vector< vector<Point> > hull(contours.size());




			for (int j = 0; j < contours[i].size(); j++) {
				convexHull(Mat(contours[i]), hull[i], false);
			}
			for (int j = 0; j < contours[i].size(); j++) {
				origin.data[contours[i][j].y*origin.cols + contours[i][j].x] = 255;
			}


			fillConvexPoly(origin, hull[i], Scalar(255, 255, 255));
		}
		//RoiImage1 = origin(cv::Rect(182, 107, 52, 14));
		//RoiImage2 = origin(cv::Rect(344, 95, 23, 24));
		int cnt = 0;
		int sum = 0;
		for (int y = 105; y < 122; y++)
		{
			for (int x = 180; x < 235; x++)
			{
				if (origin.data[y*origin.cols + x] > 250) {
					for (int m = -1; m < sample_size; m++) {
						for (int n = -1; n < sample_size; n++) {
							if ( 70 <  origin.data[(y + n)*origin.cols + (x + m)&& origin.data[y*origin.cols + x] != 255]) {
								sum += origin.data[(y+n)*origin.cols + (x+m)];
								cnt++;
							}

						}
					}
					if (cnt != 0)
						origin.data[y*origin.cols + x] = (float)sum / cnt;

				}
			}
		}

		Canny(origin(cv::Rect(344, 95, 23, 24)), dst, 70, 110, 3);
		findContours(dst, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(344, 95));

		for (int i = 0; i < contours.size(); i++) {

			Point standard, point1, point2;

			vector< vector<Point> > hull(contours.size());


			for (int j = 0; j < contours[i].size(); j++) {
				convexHull(Mat(contours[i]), hull[i], false);
			}
			for (int j = 0; j < contours[i].size(); j++) {
				origin.data[contours[i][j].y*origin.cols + contours[i][j].x] = 255;
			}

			fillConvexPoly(origin, hull[i], Scalar(255, 255, 255));
		}
		//RoiImage1 = origin(cv::Rect(182, 107, 52, 14));
		//RoiImage2 = origin(cv::Rect(344, 95, 23, 24));
		 cnt = 0;
		 sum = 0;
		for (int y = 93; y < 121; y++)
		{
			for (int x = 342; x < 368; x++)
			{
				if (origin.data[y*origin.cols + x] > 250) {
					for (int m = -1; m < sample_size; m++) {
						for (int n = -1; n < sample_size; n++) {
							if (70 <  origin.data[(y + n)*origin.cols + (x + m)&& origin.data[y*origin.cols + x] != 255]) {
								sum += origin.data[(y + n)*origin.cols + (x + m)];
								cnt++;
							}

						}
					}
					if (cnt != 0)
						origin.data[y*origin.cols + x] = (float)sum / cnt;

				}
			}
		}

		//
		//resize(origin(cv::Rect(179, 104, 58, 20)), temp, Size(58 / 2, 20 / 2), INTER_LINEAR);
		//resize(temp, temp, Size(58, 20), INTER_LINEAR);
		//resize(temp, temp, Size(58 / 2, 20 / 2), INTER_LANCZOS4);
		//resize(temp, temp, Size(58, 20), INTER_LANCZOS4);
		//temp.copyTo(origin(cv::Rect(179, 104, 58, 20)), temp);

	
		//resize(origin(cv::Rect(341, 92, 30, 30)), temp2, Size(30 / 2, 30 / 2), INTER_LINEAR);
		//resize(temp2, temp2, Size(30, 30), INTER_LINEAR);
		//resize(temp2, temp2, Size(30 / 2, 30 / 2), INTER_LANCZOS4);
		//resize(temp2, temp2, Size(30, 30), INTER_LANCZOS4);
		//temp2.copyTo(origin(cv::Rect(341, 92, 30, 30)), temp2);

		QueryPerformanceCounter(&end); 
		printf("%lf msec\n", (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000);

		avgTime += (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000;
		count++;
		printf("평균 시간: %lf msec\n", avgTime / count);

		imshow("dst", dst);

		*vp2 << origin;


		//cout << "\n";
     		if (cv::waitKey(1) == 27) {
			cv::destroyWindow("video"); break;//ESC를 누르면 종료
			delete vp2;
			}
	}


	delete vp2;
	return 0;
}
