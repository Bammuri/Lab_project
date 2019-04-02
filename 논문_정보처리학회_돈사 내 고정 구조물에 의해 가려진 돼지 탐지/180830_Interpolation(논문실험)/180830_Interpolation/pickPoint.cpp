#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>  
#include <algorithm>

using namespace cv;
using namespace std;

#define sample_size 2
#define undefined 200


bool cmp1(const Point &p1, const Point &p2) {
	if (p1.x < p2.x) {
		return true;
	}
	else if (p1.x == p2.x) {
		return p1.y < p2.y;
	}
	else {
		return false;
	}
}

bool cmp2(const Point &p1, const Point &p2) {
	if (p1.x < p2.x) {
		return true;
	}
	else if (p1.x == p2.x) {
		return p1.y > p2.y;
	}
	else {
		return false;
	}
}

bool cmp3(const Point &p1, const Point &p2) {
	if (p1.x < p2.x) {
		return true;
	}
	else if (p1.x == p2.x) {
		return p1.y > p2.y;
	}
	else {
		return false;
	}
}

int main()
{

	cv::VideoCapture Video("D:\\2018-08-18-10-25-17-Depth_Video_64.avi");//video파일을 불러와서 
	if (!Video.isOpened())//찾지 못하였을 경우 
	{
		return 0; //종료
	}

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;


	cv::Mat origin, dst, dst1;
	cv::Mat Grayscale1, Grayscale;
	cv::Mat col1_1 = imread("D:\\img_25.jpg", 0);
	cv::Mat clean_1 = imread("D:\\img_1112.bmp", 0);

	
	while (true) {


		Video >> origin; //video에서 프레임을 받아와 넘겨준다


		cvtColor(origin, Grayscale, CV_BGR2GRAY);
		cvtColor(origin, Grayscale1, CV_BGR2GRAY);

		cv::Mat clearimg = Grayscale1.clone();

		cv::Mat clearimgRoi = Grayscale1(cv::Rect(367, 335, col1_1.cols, col1_1.rows));

		clean_1.copyTo(clearimgRoi, clean_1);

		//imshow("clearimg", Grayscale1);



		//cv::Mat Grayscale_origin = Grayscale.clone();

		//imshow("Grayscale_origin", Grayscale_origin);

	

		//cv::Mat GrayscaleRoI = Grayscale(cv::Rect(367, 335, col1_1.cols, col1_1.rows));

		//col1_1.copyTo(GrayscaleRoI, col1_1);

		//imshow("Grayscale", Grayscale);



		//Mat diff;
		//cv::absdiff(Grayscale, Grayscale_origin, diff);
		//diff = Grayscale_origin - Grayscale;
		//cv::absdiff(Grayscale_origin, Grayscale, diff);

		//imshow("diff", diff);


		//dst = Grayscale1 + diff;

		//imshow("dst11", Grayscale1);

		Canny(Grayscale1(cv::Rect(367, 335, col1_1.cols, col1_1.rows)), dst1, 30, 60, 3);

		findContours(dst1, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(367, 335));

		drawContours(Grayscale1, contours, -1, (0, 0, 80),1);

	
	
		

		for (int i = 0; i < contours.size(); i++) {

			Point standard, point1, point2;
			bool flag = true;
			bool ld = false;
			bool lu = false;
			bool rd = false;
			bool ru = false;

			for (int j = 0; j < contours[i].size(); j++) {

				if (contours[i].size() == 2) {
					if(contours[i][0].y== contours[i][1].y)
					circle(Grayscale1, Point((contours[i][0].x+contours[i][1].x)/2,contours[i][0].y), abs(contours[i][0].x - contours[i][1].x) / 2, Scalar(70, 70, 70),  -1);
					else circle(Grayscale1, Point((contours[i][0].y + contours[i][1].y)/2, contours[i][0].y), abs(contours[i][0].x- contours[i][1].x) / 2, Scalar(70, 70, 70), -1);
				}
				
				if (contours[i].size() == 5) {
					if (contours[i][j].y == 384 && contours[i][j].x == 453) {
						  sort(contours[i].begin(), contours[i].end(), cmp2);
				
					}
					else if (contours[i][j].y == 384 && contours[i][j].x == 367) {
						 sort(contours[i].begin(), contours[i].end(), cmp1);
					
					}
					else if (contours[i][j].y == 335 && contours[i][j].x == 453) {
						 sort(contours[i].begin(), contours[i].end(), cmp1);
			
					}
					else if (contours[i][j].y == 335 && contours[i][j].x == 367) {
					 sort(contours[i].begin(), contours[i].end(), cmp2);
						
					}

				}

				if (contours[i].size() == 8) {

					if (contours[i][j].y == 384 && contours[i][j].x == 453) {//왼쪽 아래
						if (flag) {
							rd = true; flag = false; cout << "11" << " "; continue;
						}

						if (ru == true) { sort(contours[i].begin(), contours[i].end(), cmp1); cout << "13" << " ";
						}
						else {
							sort(contours[i].begin(), contours[i].end(), cmp2); cout << "14" << " "; std::cout << contours[i][j] << " ";;
						}
						cout << "1" << " ";
					}
					else if ((contours[i][j].y == 384) && (contours[i][j].x == 367)) {//왼쪽 아래
						if (flag){
							ld = true; flag = false; cout << "22" << " "; continue;
						}

						if (lu == true) sort(contours[i].begin(), contours[i].end(), cmp1);
						else sort(contours[i].begin(), contours[i].end(), cmp2);
						cout << "2" <<contours[i][j] << " ";
					}
					else if (contours[i][j].y == 335 && contours[i][j].x == 453) {//오른쪽 위
						if (flag){
							ru = true; flag = false; cout << "33" << " "; continue;
						}

						if (rd == true) sort(contours[i].begin(), contours[i].end(), cmp1);
						else sort(contours[i].begin(), contours[i].end(), cmp2);
						cout << "3" << " ";
						
					}
					else if (contours[i][j].y == 335 && contours[i][j].x == 367) {//왼쪽 위
						if (flag) {
							lu = true; flag = false;  cout << "44" << " "; continue;
						}

						if(ru==true) sort(contours[i].begin(), contours[i].end(), cmp2);
						else sort(contours[i].begin(), contours[i].end(), cmp1);
					
						cout << "4" <<contours[i][j] << " ";
					}

				}
				

			}
			
	


				if (contours[i].size() == 5) {

					fillConvexPoly(Grayscale1, contours[i],Scalar(70, 70, 70));



				}

				if (contours[i].size() == 8) {

					fillConvexPoly(Grayscale1, contours[i], Scalar(70, 70, 70));
				}


				for (int j = 0; j < contours[i].size(); j++) {
					cout << contours[i][j] << " ";
				}

			

			
			
			cout << "\n";
		}
		cout << "\n";






		//sort(contours[i].begin(), contours[i].end(), cmp);




	

		imshow("dst21", Grayscale1);


		if (cv::waitKey(0) == 27) {
			cv::destroyWindow("video"); break;//ESC를 누르면 종료
		}
	}


	return 0;
}
