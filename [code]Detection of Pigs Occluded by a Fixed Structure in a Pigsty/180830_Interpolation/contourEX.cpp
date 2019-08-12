#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>  

using namespace cv;
using namespace std;

#define sample_size 2
#define undefined 200



int main()
{

	cv::VideoCapture Video("Test123.avi");//video파일을 불러와서 
	if (!Video.isOpened())//찾지 못하였을 경우 
	{
		return 0; //종료
	}


	cv::Mat origin, dst2;
	cv::Mat Grayscale;



	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;


	while (true) {


		Video >> origin; //video에서 프레임을 받아와 넘겨준다


		cvtColor(origin, Grayscale, CV_BGR2GRAY);

		Mat dst = Mat::zeros(origin.rows, origin.cols, CV_8UC1);
		cv::Mat Grayscale_origin = Grayscale.clone();


		imshow("Grayscale_origin", Grayscale_origin);


		//Mat im_th;
		//threshold(Grayscale_origin, im_th, 220, 255, THRESH_BINARY_INV);

		//// Floodfill from point (0, 0)
		//Mat im_floodfill = im_th.clone();
		//floodFill(im_floodfill, cv::Point(0, 0), Scalar(255));


		//// Invert floodfilled image
		//Mat im_floodfill_inv;
		//bitwise_not(im_floodfill, im_floodfill_inv);

		//// Combine the two images to get the foreground.
		//Mat im_out = (im_th | im_floodfill_inv);

		//// Display images
		//imshow("Thresholded Image", im_th);
		//imshow("Floodfilled Image", im_floodfill);
		//imshow("Inverted Floodfilled Image", im_floodfill_inv);
		//imshow("Foreground", im_out);

		Canny(Grayscale_origin, dst, 5, 60, 3);

		findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point());

	


		//cv::drawContours(dst, contours,	-1,       // 모든 외곽선 그리기cv::Scalar(255, 255, 255), // 검게2);

		/*for (int i = 0; i < contours.size();i++) {
			for (int j = 0; j < contours[i].size();j++) {
				cout << contours[i][j] << " ";
			}
		}*/
		imshow("dst", dst);


		cv::waitKey(0);
	
		
	}


	return 0;
}
