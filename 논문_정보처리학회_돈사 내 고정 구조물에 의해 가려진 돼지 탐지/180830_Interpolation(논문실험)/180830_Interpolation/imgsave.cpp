#include<opencv2/opencv.hpp>
#include <iostream>  

using namespace cv;
using namespace std;

int main()
{
	//(132,118)
	cv::VideoCapture Video("D:\\���� Ž�� ����\\���ܼ�\\2018-08-18-09-53-00-Infrared_Video_62.avi");//video������ �ҷ��ͼ� 
	if (!Video.isOpened())//ã�� ���Ͽ��� ��� 
	{
		return 0; //����
	}

    cv::Mat origin;
	cv::Mat Grayscale;
	int cnt = 0;
	//cv::namedWindow("origin");//ȭ�鿡 ��� â ����
	//(132,118)

	while (true) {

		Video >> origin; //video���� �������� �޾ƿ� �Ѱ��ش�
		

		cvtColor(origin, Grayscale, CV_BGR2GRAY);
		Mat iROI = Grayscale(Rect(0, 132, 1160, 472));
		Mat iResize;
		//(132,118)
		resize(iROI, iResize, Size(iROI.cols / 4, iROI.rows / 4), 0, 0, CV_INTER_LINEAR);
	    imshow("origin", iResize);
		cv::Mat RoiImage;       
	    RoiImage = iResize(cv::Rect(171, 62, 13, 12));
		//("RoiImage", RoiImage50

		char file_name[20];

		sprintf_s(file_name, "img_%d.bmp",cnt);
		
		imwrite(file_name, RoiImage); 

		

   		imshow("dst", RoiImage);//�Ѱ��� �������� ȭ�鿡 ���

     	cnt++;
		cv::waitKey(1);

	}


	return 0;
}