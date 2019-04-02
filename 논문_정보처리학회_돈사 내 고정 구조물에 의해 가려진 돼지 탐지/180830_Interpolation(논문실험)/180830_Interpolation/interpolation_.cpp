#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>  

using namespace cv;
using namespace std;

#define sample_size 2
#define undefined 200

void SpatioTemporal(Mat &src1, Mat &src2, Mat &src3, Mat &dst);
Mat TemporalInterpolation(Mat &image1, Mat &image2, Mat &image3);
Mat SpatioInterpolation(Mat &image);

int main()
{

	VideoCapture Video("noblock.avi");//video파일을 불러와서 
	if (!Video.isOpened())//찾지 못하였을 경우 
	{
		return 0; //종료
	}

	cv::Mat origin, origin2, origin3, ROIorigin;
	cv::Mat Grayscale;
	//cv::Mat col1_1 = imread("D:\\img_25.jpg", 0);

  	Video >> origin;
    //ROIorigin = origin(Rect(0, 70, 580, 220));


	//파일로 동영상을 저장하기 위한 준비  

	int  fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D');

	VideoWriter *vp2=new VideoWriter;

	if (!vp2->open("target.avi", fourcc, 10, Rect(0,0,640,360).size(), false)) {
		delete vp2;
		return 0;
	}

	while (true) {

		Video >> origin;
		Video >> origin2;
		Video >> origin3;

		//origin = origin(Rect(0, 140, 1160, 440));
		//origin2 = origin(Rect(0, 140, 1160, 440));
		//origin3 = origin(Rect(0, 140, 1160, 440));

		cvtColor(origin, origin, CV_BGR2GRAY);
		cvtColor(origin2, origin2, CV_BGR2GRAY);
		cvtColor(origin3, origin3, CV_BGR2GRAY);

		cv::Mat sdt;

		SpatioTemporal(origin, origin2, origin3, sdt);
		imshow("sdt", sdt);

		*vp2 << sdt;

		//Video >> origin; //video에서 프레임을 받아와 넘겨준다


		//cvtColor(origin, Grayscale, CV_BGR2GRAY);

		//cv::Mat Grayscale_origin= Grayscale;
		//
		//
		//cv::Mat GrayscaleRoI = Grayscale(cv::Rect(367, 335, col1_1.cols, col1_1.rows));
	
		//col1_1.copyTo(GrayscaleRoI, col1_1);

		//

		////cvAbsDiff();

		//imshow("video", Grayscale);//넘겨준 프레임을 화면에 출력


		char key = waitKey(50);
		if ( key == 27) {
			delete vp2;
			break;//ESC를 누르면 종료
		}
	}

	delete vp2;
	return 0;
}


// src1, src2, src3, dst : Grayscale
void SpatioTemporal(Mat &src1, Mat &src2, Mat &src3, Mat &dst) {
	Mat GraySrc[3];
	Mat ResultTemp;

	if (src1.channels() == 3) {
		printf("===SpatioTemporal===\n");
		printf("The input image's channel is not grayscale\n");
		return;

	}

	GraySrc[0] = src1.clone();
	GraySrc[1] = src2.clone();
	GraySrc[2] = src3.clone();

	ResultTemp = TemporalInterpolation(GraySrc[0], GraySrc[1], GraySrc[2]);
	ResultTemp = SpatioInterpolation(ResultTemp);
	
	dst = ResultTemp;

}


Mat TemporalInterpolation(Mat &image1, Mat &image2, Mat &image3) {
	Mat Result(image1.rows, image1.cols, CV_8UC1);
	int cnt;
	int sum;

	for (int i = 0; i < Result.rows; i++) {
		for (int j = 0; j < Result.cols; j++) {
			cnt = 0;
			sum = 0;

			if (image1.data[j + i * Result.cols] < undefined) {
				cnt++;
				sum += image1.data[j + i * Result.cols];
			}
			if (image2.data[j + i * Result.cols] < undefined) {
				cnt++;
				sum += image2.data[j + i * Result.cols];
			}
			if (image3.data[j + i * Result.cols] < undefined) {
				cnt++;
				sum += image3.data[j + i * Result.cols];
			}
			if (cnt == 0) {
				sum = image1.data[j + i * image1.cols] + image2.data[j + i * image2.cols] + image3.data[j + i * image3.cols];
				cnt = 3;
			}
			Result.data[j + i * Result.cols] = (float)sum / cnt;
		}
	}

	return Result;
}


Mat SpatioInterpolation(Mat &image) {
	Mat Result(image.rows / sample_size, image.cols / sample_size, CV_8UC1);
	int cnt;
	int sum;

	for (int i = 0; i < Result.rows; i++) {
		for (int j = 0; j < Result.cols; j++) {
			cnt = 0;
			sum = 0;

			for (int m = 0; m < sample_size; m++) {
				for (int n = 0; n < sample_size; n++) {
					if (image.data[(j * sample_size + n) + (i * sample_size + m) *image.cols] < undefined) {          //undefined -> 255
						cnt++;
						sum += image.data[(j * sample_size + n) + (i * sample_size + m)*image.cols];
					}
				}
			}

			if (cnt == 0) {
				for (int m = 0; m < sample_size; m++) {
					for (int n = 0; n < sample_size; n++) {
						sum += image.data[(j * sample_size + n) + (i * sample_size + m)*image.cols];          // 복원해야 하는 부분 -> 모두 undefined 일때
																											  //sum = 255;
					}
				}
				cnt = pow(sample_size, 2);
			}

			Result.data[j + i * Result.cols] = (float)sum / cnt;
		}
	}
	return Result;
}


/*
for (int y = 0; y <dst.rows; y++)
{
	for (int x = 0; x < dst.cols; x++)
	{

		if (dst.data[y*Grayscale.cols + x] <60)
			dst.data[y*Grayscale.cols + x] = 255;

	}
}
imshow("dst1", dst);
Mat element = getStructuringElement(cv::MORPH_ELLIPSE, Size(3, 3), Point(-1, -1));



dilate(dst, dst2, element, Point(-1, -1), 1);
imshow("dst3", dst2);

erode(dst2, dst2, element, Point(-1, -1), 5);
erode(dst2, dst2, Mat::ones(3, 3, CV_8UC1), Point(-1, -1), 5);
imshow("dst2", dst2);
waitKey(0);
*/