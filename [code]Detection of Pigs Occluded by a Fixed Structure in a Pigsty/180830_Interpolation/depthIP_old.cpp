#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>  
#include <algorithm>

using namespace cv;
using namespace std;

#define sample_size 2


int main()
{

	cv::VideoCapture Video("target_origin.avi");//video파일을 불러와서 
	if (!Video.isOpened())//찾지 못하였을 경우 
	{
		return 0; //종료
	}

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;


	cv::Mat origin, dst, dst1;
	cv::Mat Grayscale1, Grayscale;
	/*Video >> origin; //video에서 프레임을 받아와 넘겨준다
	int  fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D');

	VideoWriter *vp2 = new VideoWriter;

	if (!vp2->open("dst.avi", fourcc, 10, origin.size(), false)) {
		delete vp2;
		return 0;
	}*/
	while (true) {


		Video >> origin; //video에서 프레임을 받아와 넘겨준다

		cvtColor(origin, Grayscale1, CV_BGR2GRAY);

		Canny(Grayscale1(cv::Rect(184, 104, 43, 23)), dst1, 50, 60, 3);

		findContours(dst1, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(184, 104));

		//drawContours(Grayscale1, contours, -1, (255, 255, 255), 1);

		Mat Result(Grayscale1.rows, Grayscale1.cols, CV_8UC1);
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
				Grayscale1.data[contours[i][j].y*Grayscale1.cols + contours[i][j].x] = 255;
			}


			for (int j = 0; j < contours[i].size(); j++) {
				//cout << contours[i][j] << " ";
				cnt = 0;
				sum = 0;

				//cout << (float)Grayscale1.data[contours[i][j].y*Grayscale1.cols + contours[i][j].x] << ' ';
				if (Grayscale1.data[contours[i][j].y*Grayscale1.cols + contours[i][j].x] == 255) {
					for (int m = -1; m < sample_size; m++) {
						for (int n = -1; n < sample_size; n++) {
							if (Grayscale1.data[(contours[i][j].y + m)*Grayscale1.cols + contours[i][j].x + n] != 255) {
								sum += Grayscale1.data[(contours[i][j].y + m)*Grayscale1.cols + contours[i][j].x + n];
								cnt++;
							}

						}
					}
					Grayscale1.data[contours[i][j].y*Grayscale1.cols + contours[i][j].x] = (float)sum / cnt;
					totalsum += sum / cnt;
					totalcnt++;
				}
			}
			if (totalcnt != 0)
				fillConvexPoly(Grayscale1, hull[i], Scalar((totalsum / totalcnt) + 15, (totalsum / totalcnt) + 15, (totalsum / totalcnt) + 15));


			resize(Grayscale1, Grayscale1, Size(Grayscale1.cols / 2, Grayscale1.rows / 2), INTER_LINEAR);
			resize(Grayscale1, Grayscale1, Size(Grayscale1.cols * 2, Grayscale1.rows * 2), INTER_LINEAR);
			resize(Grayscale1, Grayscale1, Size(Grayscale1.cols / 2, Grayscale1.rows / 2), INTER_LANCZOS4);
			resize(Grayscale1, Grayscale1, Size(Grayscale1.cols * 2, Grayscale1.rows * 2), INTER_LANCZOS4);
			resize(Grayscale1, Grayscale1, Size(Grayscale1.cols / 2, Grayscale1.rows / 2), INTER_CUBIC);
			resize(Grayscale1, Grayscale1, Size(Grayscale1.cols * 2, Grayscale1.rows * 2), INTER_CUBIC);

			//blur(Grayscale1, Grayscale1, Size(5, 5));
			cout << "\n";
		}
		imshow("dst21", Grayscale1);
		//*vp2 << Grayscale1;


		cout << "\n";
		if (cv::waitKey(1) == 27) {
			cv::destroyWindow("video"); break;//ESC를 누르면 종료
		//	delete vp2;
		}
	}
	//delete vp2;
	return 0;
}