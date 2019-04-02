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

	cv::VideoCapture Video("D:\\���� Ž�� ����\\����\\2018-08-18-10-56-34-Depth_Video_66_intpl_depth.avi");//video������ �ҷ��ͼ� 
	if (!Video.isOpened())//ã�� ���Ͽ��� ��� 
	{
		return 0; //����
	}
	cv::Mat clean_1 = imread("img_15.bmp", 0);
	cv::Mat clean_2 = imread("img_1.bmp", 0);

	cv::Mat origin, dst;
	cv::Mat Grayscale;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Video >> origin;
	int fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D');

	VideoWriter *vp2 = new VideoWriter;

	if (!vp2->open("2018-08-18-10-56-34-Depth_Video_66_intpl_depth.avi", fourcc, 10, origin.size(), false)) {
		delete vp2;
		return 0;
	}
	double avgTime = 0;
	int count = 0;



	while (true) {
		LARGE_INTEGER freq, start, end;

		Video >> origin; //video���� �������� �޾ƿ� �Ѱ��ش�


		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start);

		cvtColor(origin, origin, CV_BGR2GRAY);


		//imshow("origin", origin);

		cv::Mat RoiImage1;
		cv::Mat RoiImage2;

		RoiImage1 = origin(cv::Rect(184, 104, 43, 23));
		RoiImage2 = origin(cv::Rect(344, 99, 32, 24));

		clean_1.copyTo(RoiImage1, clean_1);
		clean_2.copyTo(RoiImage2, clean_2);
		//imshow("clearimg", origin);

	

		Canny(origin(cv::Rect(184, 104, 43, 23)), dst, 50, 60, 3);

		findContours(dst, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(184, 104));

		Canny(origin(cv::Rect(344, 99, 32, 24)), dst, 50, 60, 3);

	
		for (int i = 0; i < contours.size(); i++) {

			Point standard, point1, point2;

			vector< vector<Point> > hull(contours.size());

			int cnt;
			int sum;
			int totalcnt = 0;
			int totalsum = 0;

			for (int j = 0; j < contours[i].size(); j++) {
				convexHull(Mat(contours[i]), hull[i], false);
			}
			for (int j = 0; j < contours[i].size(); j++) {
				origin.data[contours[i][j].y*origin.cols + contours[i][j].x] = 255;
			}


			for (int j = 0; j < contours[i].size(); j++) {
				//cout << contours[i][j] << " ";
				cnt = 0;
				sum = 0;

				//cout << (float)Grayscale1.data[contours[i][j].y*Grayscale1.cols + contours[i][j].x] << ' ';
				if (origin.data[contours[i][j].y*origin.cols + contours[i][j].x] == 255) {
					for (int m = -1; m < sample_size; m++) {
						for (int n = -1; n < sample_size; n++) {
							if (origin.data[(contours[i][j].y + m)*origin.cols + contours[i][j].x + n] != 255) {
								sum += origin.data[(contours[i][j].y + m)*origin.cols + contours[i][j].x + n];
								cnt++;
							}

						}
					}
					if (cnt != 0) {
						origin.data[contours[i][j].y*origin.cols + contours[i][j].x] = (float)sum / cnt;
						totalsum += sum / cnt;
					}
					totalcnt++;
				}
			}
			if (totalcnt != 0)
				fillConvexPoly(origin, hull[i], Scalar((totalsum / totalcnt) + 15, (totalsum / totalcnt) + 15, (totalsum / totalcnt) + 15));



			/*resize(origin, origin, Size(origin.cols / 2, origin.rows / 2), INTER_LINEAR);
			resize(origin, origin, Size(origin.cols * 2, origin.rows * 2), INTER_LINEAR);
			resize(origin, origin, Size(origin.cols / 2, origin.rows / 2), INTER_LANCZOS4);
			resize(origin, origin, Size(origin.cols * 2, origin.rows * 2), INTER_LANCZOS4);
			resize(origin, origin, Size(origin.cols / 2, origin.rows / 2), INTER_CUBIC);
			resize(origin, origin, Size(origin.cols * 2, origin.rows * 2), INTER_CUBIC);
*/
			//blur(origin, origin, Size(3, 3));
			//cout << "\n";
		}


		
		findContours(dst, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(344, 99));


		
		for (int i = 0; i < contours.size(); i++) {

			Point standard, point1, point2;

			vector< vector<Point> > hull(contours.size());

			int cnt;
			int sum;
			int totalcnt = 0;
			int totalsum = 0;

			for (int j = 0; j < contours[i].size(); j++) {
				convexHull(Mat(contours[i]), hull[i], false);
			}

			for (int j = 0; j < contours[i].size(); j++) {
				origin.data[contours[i][j].y*origin.cols + contours[i][j].x] = 255;
			}

			for (int j = 0; j < contours[i].size(); j++) {
				//cout << contours[i][j] << " ";
				cnt = 0;
				sum = 0;

				//cout << (float)Grayscale1.data[contours[i][j].y*Grayscale1.cols + contours[i][j].x] << ' ';
				if (origin.data[contours[i][j].y*origin.cols + contours[i][j].x] == 255) {
					for (int m = -1; m < sample_size; m++) {
						for (int n = -1; n < sample_size; n++) {
							if (origin.data[(contours[i][j].y + m)*origin.cols + contours[i][j].x + n] != 255) {
								sum += origin.data[(contours[i][j].y + m)*origin.cols + contours[i][j].x + n];
								cnt++;
							}

						}
					}
					if (cnt != 0) {
						origin.data[contours[i][j].y*origin.cols + contours[i][j].x] = (float)sum / cnt;
						totalsum += sum / cnt;
					}
					totalcnt++;
				}
			}
			if (totalcnt != 0)
				fillConvexPoly(origin, hull[i], Scalar((totalsum / totalcnt) + 15, (totalsum / totalcnt) + 15, (totalsum / totalcnt) + 15));






		
			//blur(origin, origin, Size(3, 3));
			//cout << "\n";
		}
		resize(origin, origin, Size(origin.cols / 2, origin.rows / 2), INTER_LINEAR);
		resize(origin, origin, Size(origin.cols * 2, origin.rows * 2), INTER_LINEAR);
		resize(origin, origin, Size(origin.cols / 2, origin.rows / 2), INTER_LANCZOS4);
		resize(origin, origin, Size(origin.cols * 2, origin.rows * 2), INTER_LANCZOS4);
		resize(origin, origin, Size(origin.cols / 2, origin.rows / 2), INTER_CUBIC);
		resize(origin, origin, Size(origin.cols * 2, origin.rows * 2), INTER_CUBIC);


		QueryPerformanceCounter(&end);
		printf("%lf msec\n", (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000);

		avgTime += (double)(end.QuadPart - start.QuadPart) / freq.QuadPart * 1000;
		count++;
		printf("��� �ð�: %lf msec\n", avgTime / count);


		imshow("dst", origin);


		*vp2 << origin;


		//cout << "\n";
		if (cv::waitKey(1) == 27) {
			cv::destroyWindow("video"); break;//ESC�� ������ ����
			delete vp2;
		}
	}
	delete vp2;
	return 0;
}
