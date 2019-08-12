#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>  
#include <Windows.h>
using namespace cv;
using namespace std;


#define sample_size 2

int main()
{

	cv::VideoCapture Video("D:\\보간 탐지 영상\\적외선\\Intpl_2018-08-18-10-09-05-Infrared_Video_63.avi");//video파일을 불러와서 
	if (!Video.isOpened())//찾지 못하였을 경우 
	{
		return 0; //종료
	}
	cv::Mat clean_1 = imread("img_134.bmp", 0);
	cv::Mat clean_2 = imread("img_9.bmp", 0);

	cv::Mat origin, dst; zeros
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

	
		//resize(origin, origin, Size(origin.cols / 2, origin.rows / 2), INTER_LINEAR);
		//resize(origin, origin, Size(origin.cols * 2, origin.rows * 2), INTER_LINEAR);
		//resize(origin, origin, Size(origin.cols / 2, origin.rows / 2), INTER_LANCZOS4);
		//resize(origin, origin, Size(origin.cols * 2, origin.rows * 2), INTER_LANCZOS4);
		//resize(origin, origin, Size(origin.cols / 2, origin.rows / 2), INTER_CUBIC);
		//resize(origin, origin, Size(origin.cols * 2, origin.rows * 2), INTER_CUBIC);
		//
		Canny(origin(cv::Rect(182, 107, 52, 14)), dst, 50, 255, 3);
		

	
		//Canny(origin(cv::Rect(344, 95, 23, 24)), dst, 50, 255, 3);
		



		for (int y = 106; y <121; y++)
		{
			for (int x = 181; x < 234; x++)
			{

				
				if()
				origin.data[y*origin.cols + x] = (origin.data[(y - 1) * origin.cols + (x - 1)] + origin.data[y*origin.cols + (x - 1)] + origin.data[(y - 1) * origin.cols + x]) / 3;
					

			}
		}



		Mat temp;
		resize(origin(cv::Rect(180, 105, 56, 18)) , temp, Size(56 / 2, 18 / 2), INTER_LINEAR);
		resize(temp, temp, Size( 56, 18), INTER_LINEAR);
		temp.copyTo(origin(cv::Rect(180, 105, 56, 18)), temp);
		//resize(origin(cv::Rect(181, 106, 54, 16)); , origin, Size(origin.cols / 2, origin.rows / 2), INTER_LANCZOS4);
		//resize(origin(cv::Rect(181, 106, 54, 16)); , origin, Size(origin.cols * 2, origin.rows * 2), INTER_LANCZOS4);
		//resize(origin(cv::Rect(181, 106, 54, 16)); , origin, Size(origin.cols / 2, origin.rows / 2), INTER_CUBIC);
		//resize(origin(cv::Rect(181, 106, 54, 16)); , origin, Size(origin.cols * 2, origin.rows * 2), INTER_CUBIC);

		QueryPerformanceCounter(&end);
		printf("%lf msec\n", (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000);

		avgTime += (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000;
		count++;
		printf("평균 시간: %lf msec\n", avgTime / count);

		imshow("dst", origin);

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
