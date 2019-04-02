#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {

	VideoCapture cap("Test123.avi");

	Mat src;

	//cap.set(CAP_PROP_POS_FRAMES, 540);

	while (1) {
		cap >> src;

		Mat gray, edgeImg;

		cvtColor(src, gray, CV_BGR2GRAY);

		//medianBlur( gray, gray, 5 );
		GaussianBlur(gray, gray, Size(5, 5), 6);
		imshow("origin image", gray);

		equalizeHist(gray, gray);
		imshow("hist image", gray);
		imwrite("histImg2.bmp", gray);

		Canny(gray, edgeImg, 40, 250, 3);
		//Laplacian(gray, edgeImg, CV_8U, 5);

		imshow("edge image", edgeImg);
		equalizeHist(edgeImg, edgeImg);

		imshow("hist image2", edgeImg);

		char ch = waitKey(0);
		if (ch == 27)
			break;
	}

	return 0;
}