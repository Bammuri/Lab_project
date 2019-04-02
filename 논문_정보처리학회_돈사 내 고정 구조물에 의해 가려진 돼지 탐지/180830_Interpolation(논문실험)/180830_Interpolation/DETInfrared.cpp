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

	cv::Mat clean_L = imread("C:\\Users\\YANG SUN MIN\\Desktop\\적외선\\img_676a.bmp", 0);
	cv::Mat sample_L = imread("C:\\Users\\YANG SUN MIN\\Desktop\\적외선\\img_190.bmp", 0);

	cv::Mat clean_R = imread("C:\\Users\\YANG SUN MIN\\Desktop\\적외선\\img_1097a.bmp", 0);
	cv::Mat sample_R = imread("C:\\Users\\YANG SUN MIN\\Desktop\\적외선\\img_1034.bmp", 0);

	cv::Mat origin, dst;
	cv::Mat Grayscale;

	vector<vector<Point> > contours1;
	vector<Vec4i> hierarchy1;
	vector<vector<Point> > contours2;
	vector<Vec4i> hierarchy2;
	Video >> origin;
	int fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D');

	VideoWriter *vp2 = new VideoWriter;

	if (!vp2->open("aaaaaIntpl_2018-08-18-10-09-05-Infrared_Video_63.avi", fourcc, 10, origin.size(), false)) {
		delete vp2;
		return 0;
	}

	double avgTime = 0;
	int count = 0;
	int cnt1 = 0;

	int tempCount = 1;

	while (true) {
		LARGE_INTEGER freq, start, end;

		Video >> origin; //video에서 프레임을 받아와 넘겨준다

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start);

		cvtColor(origin, origin, CV_BGR2GRAY);


		//imshow("origin", origin);

		cv::Mat RoiImage1;
		cv::Mat RoiImage2;

		cv::Mat sample_origin = origin.clone();
		RoiImage1 = sample_origin(cv::Rect(182, 107, 52, 14));
		sample_L.copyTo(RoiImage1, sample_L);

		RoiImage2 = sample_origin(cv::Rect(335, 103, 32, 17));
		sample_R.copyTo(RoiImage2, sample_R);

		//imshow("sample_origin", sample_origin);
		
		Mat block = Mat::zeros(origin.rows, origin.cols, CV_8UC1);
		RoiImage1 = block(cv::Rect(182, 107, 52, 14));
		clean_L.copyTo(RoiImage1, clean_L);

		RoiImage2 = block(cv::Rect(335, 103, 32, 17));
		clean_R.copyTo(RoiImage2, clean_R);

		//imshow("origin2", block);

		dst = origin - block;
	
		
		imshow("dst11", dst);

		for (int y = 107; y < 121; y++)
		{
			for (int x = 182; x < 234; x++)
			{
				if (dst.data[y*dst.cols + x] < 30) {
					dst.data[y*dst.cols + x] = sample_origin.data[y*sample_origin.cols + x];
				}


			}
		}


		for (int y = 103; y < 120; y++)
		{
			for (int x = 335; x < 367; x++)
			{

				if (dst.data[y*dst.cols + x] < 30) {
					dst.data[y*dst.cols + x] = sample_origin.data[y*sample_origin.cols + x];
				}

			}
		}
		cv::Mat canny = dst.clone();
		//imshow("dst12", dst);

		//RoiImage1 = origin(cv::Rect(182, 107, 52, 14));
		//RoiImage2 = origin(cv::Rect(335, 103, 32, 17));
		//imshow("dst1", dst);
		Mat temp;
		resize(dst(cv::Rect(179, 104, 58, 20)), temp, Size(58 / 2, 20 / 2), INTER_LINEAR);
		resize(temp, temp, Size(58, 20), INTER_LINEAR);
		resize(temp, temp, Size(58 / 2, 20 / 2), INTER_LANCZOS4);
		resize(temp, temp, Size(58, 20), INTER_LANCZOS4);
		resize(temp, temp, Size(58 / 2, 20 / 2), INTER_LANCZOS4);
		resize(temp, temp, Size(58, 20), INTER_LANCZOS4);
		temp.copyTo(dst(cv::Rect(179, 104, 58, 20)), temp);
		
		

		Mat CannyTemp;
		Canny(dst(cv::Rect(183, 108, 50, 12)), CannyTemp, 60, 110, 3);
		findContours(CannyTemp, contours1, hierarchy1, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(183, 108));
		CannyTemp.copyTo(canny(cv::Rect(183, 108, 50, 12)), CannyTemp);
		//imshow("CannyTemp", dst);

		for (int i = 0; i < contours1.size(); i++) {

			Point standard, point1, point2;

			vector< vector<Point> > hull(contours1.size());




			for (int j = 0; j < contours1[i].size(); j++) {
				convexHull(Mat(contours1[i]), hull[i], false);
			}



			fillConvexPoly(dst, hull[i], Scalar(255, 255, 255));
		}
		//cvtColor(dst, dst, CV_BGR2GRAY);
		//imshow("dst13", dst);







		Mat temp2;
		resize(dst(cv::Rect(332, 100, 38, 24)), temp2, Size(38 / 2, 24 / 2), INTER_LINEAR);
		resize(temp2, temp2, Size(38, 24), INTER_LINEAR);
		resize(temp2, temp2, Size(38 / 2, 24 / 2), INTER_LANCZOS4);
		resize(temp2, temp2, Size(38, 24), INTER_LANCZOS4);
		resize(temp2, temp2, Size(38 / 2, 24 / 2), INTER_LANCZOS4);
		resize(temp2, temp2, Size(38, 24), INTER_LANCZOS4);
		temp2.copyTo(dst(cv::Rect(332, 100, 38, 24)), temp2);

		Mat CannyTemp1;
		Canny(dst(cv::Rect(335, 103, 32, 17)), CannyTemp1, 60, 110, 3);
		findContours(CannyTemp1, contours2, hierarchy2, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(335, 103));
		CannyTemp1.copyTo(canny(cv::Rect(335, 103, 32, 17)), CannyTemp1);
		//imshow("CannyTemp", canny);
		
		
		for (int i = 0; i < contours2.size(); i++) {

			Point standard, point1, point2;

			vector< vector<Point> > hull(contours2.size());




			for (int j = 0; j < contours2[i].size(); j++) {
				convexHull(Mat(contours2[i]), hull[i], false);
			}



			fillConvexPoly(dst, hull[i], Scalar(255, 255, 255));
		}
		//imshow("whatthe", dst);

		int cnt = 0;
		int sum = 0;
		for (int y = 107; y < 121; y++)
		{
			for (int x = 182; x < 234; x++)
			{
				if (dst.data[y*dst.cols + x] == 255) {

					for (int m = -1; m < sample_size; m++) {
						for (int n = -1; n < sample_size; n++) {
							if (dst.data[(y + n)*dst.cols + (x + m)] != 255) {
								sum += dst.data[(y + n)*dst.cols + (x + m)];
								cnt++;
							}

						}
					}
					if (cnt != 0)
						dst.data[y*dst.cols + x] = (float)(sum / cnt) + 15;

				}



			}
		}

		resize(dst(cv::Rect(179, 104, 58, 20)), temp, Size(58 / 2, 20 / 2), INTER_LINEAR);
		resize(temp, temp, Size(58, 20), INTER_LINEAR);
		resize(temp, temp, Size(58 / 2, 20 / 2), INTER_LANCZOS4);
		resize(temp, temp, Size(58, 20), INTER_LANCZOS4);
		temp.copyTo(dst(cv::Rect(179, 104, 58, 20)), temp);

	




		cnt = 0;
		sum = 0;
		for (int y = 103; y < 120; y++)
		{
			for (int x = 335; x < 367; x++)
			{
				if (dst.data[y*dst.cols + x] == 255) {

					for (int m = -1; m < sample_size; m++) {
						for (int n = -1; n < sample_size; n++) {
							if (dst.data[(y + n)*dst.cols + (x + m)] != 255) {
								sum += dst.data[(y + n)*dst.cols + (x + m)];
								cnt++;
							}

						}
					}
					if (cnt != 0)
						dst.data[y*dst.cols + x] = (float)(sum / cnt) + 30;

				}



			}
		}

		resize(dst(cv::Rect(332, 100, 38, 24)), temp2, Size(38 / 2, 24 / 2), INTER_LINEAR);
		resize(temp2, temp2, Size(38, 24), INTER_LINEAR);
		resize(temp2, temp2, Size(38 / 2, 24 / 2), INTER_LANCZOS4);
		resize(temp2, temp2, Size(38, 24), INTER_LANCZOS4);
		temp2.copyTo(dst(cv::Rect(332, 100, 38, 24)), temp2);


		imshow("dst", dst);
		


		char file_name[20];

		sprintf_s(file_name, "img_%d.bmp", cnt1);

		imwrite(file_name, dst);
		cnt1++;
		QueryPerformanceCounter(&end);
		printf("%lf msec\n", (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000);

		avgTime += (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000;
		count++;
		printf("평균 시간: %lf msec\n", avgTime / count);



		*vp2 << dst;

		cout << "Frame number: " << tempCount++ << endl;

		//cout << "\n";
		if (cv::waitKey(1) == 27) {
			cv::destroyWindow("video"); break;//ESC를 누르면 종료
			delete vp2;
		}
	}


	delete vp2;
	return 0;
}
