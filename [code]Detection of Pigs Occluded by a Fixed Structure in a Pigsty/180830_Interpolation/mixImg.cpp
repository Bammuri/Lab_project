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

	cv::VideoCapture Video("D:\\보간 탐지 영상\\깊이\\2018-08-18-09-53-00-Depth_Video_62_intpl_depth.avi");//video파일을 불러와서 
	if (!Video.isOpened())//찾지 못하였을 경우 
	{
		return 0; //종료
	}


	int  fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D');

	cv::Mat origin,dst,dst2;
	cv::Mat Grayscale1,Grayscale;
	cv::Mat col1_1 = imread("img_351.bmp", 0);
	cv::Mat clean_1 = imread("img_0.bmp", 0);
	Video >> origin;
	VideoWriter *vp2 = new VideoWriter;
	if (!vp2->open("123.avi", fourcc, 10, origin.size(), false)) {
		delete vp2;
		return 0;
	}
	while (true) {


		Video >> origin; //video에서 프레임을 받아와 넘겨준다


		cvtColor(origin, Grayscale, CV_BGR2GRAY);
		cvtColor(origin, Grayscale1, CV_BGR2GRAY);

		cv::Mat clearimg=Grayscale1.clone();

		cv::Mat clearimgRoi = Grayscale1(cv::Rect(184, 104, 43, 23));

		clean_1.copyTo(clearimgRoi, clean_1);

		imshow("clearimg", Grayscale1);
		*vp2 << Grayscale1;


		//cv::Mat Grayscale_origin= Grayscale.clone();

		//imshow("Grayscale_origin", Grayscale_origin);
		
		//cv::Mat GrayscaleRoI = Grayscale(cv::Rect(184, 104, 43, 23));

		//col1_1.copyTo(GrayscaleRoI, col1_1);

		//imshow("Grayscale", Grayscale);



		//Mat diff;
		//cv::absdiff(Grayscale, Grayscale_origin, diff);
		//diff = Grayscale_origin - Grayscale;
		//cv::absdiff(Grayscale_origin, Grayscale, diff);

		//imshow("diff", diff);

		
		//dst = Grayscale1 + diff;

		//imshow("dst11", dst);

		//resize(dst, dst, Size(origin.cols / 2, origin.rows / 2), INTER_LANCZOS4);
		//resize(dst, dst, Size(origin.cols , origin.rows ), INTER_LANCZOS4);
		//imshow("dst111", dst);
		//*vp2 << dst;
		//cv::Mat GrayscaleRoI = Grayscale(cv::Rect(367, 335, col1_1.cols, col1_1.rows));



	
		if (cv::waitKey(10) == 27) {
			cv::destroyWindow("video"); break;//ESC를 누르면 종료
				delete vp2;
		}
	}

	delete vp2;
	return 0;
}
