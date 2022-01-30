#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<bits/stdc++.h>

using namespace std;
using namespace cv;

#define CLK_TCK  CLOCKS_PER_SEC //机器时钟每秒的打点数
clock_t Begin, End;
double duration[2];

Mat img, image_gray, dx, dy, dxx, dyy, dxy, C, C_opencv, dxx_gauss, dyy_gauss, dxy_gauss, image_my, image_opencv;
int threshold1, threshold2;

//Mat SobelXX(const Mat src) {
//	int row = src.rows;
//	int col = src.cols;
//	Mat_<float> dst(row, col, CV_32FC1);
//	for (int i = 0; i < row; i++) {
//		for (int j = 0; j < col; j++) {
//			dst.at<float>(i, j) = src.at<uchar>(i, j) * src.at<uchar>(i, j);
//		}
//	}
//	return dst;
//}
//
//Mat SobelYY(const Mat src) {
//	int row = src.rows;
//	int col = src.cols;
//	Mat_<float> dst(row, col, CV_32FC1);
//	for (int i = 0; i < row; i++) {
//		for (int j = 0; j < col; j++) {
//			dst.at<float>(i, j) = src.at<uchar>(i, j) * src.at<uchar>(i, j);
//		}
//	}
//	return dst;
//}
//
//Mat SobelXY(const Mat src1, const Mat& src2) {
//	int row = src1.rows;
//	int col = src1.cols;
//	Mat_<float> dst(row, col, CV_32FC1);
//	for (int i = 0; i < row; i++) {
//		for (int j = 0; j < col; j++) {
//			dst.at<float>(i, j) = src1.at<uchar>(i, j) * src2.at<uchar>(i, j);
//		}
//	}
//	return dst;
//}
//
//Mat harrisResponse(Mat_<float> GaussXX, Mat_<float> GaussYY, Mat_<float> GaussXY, float k) {
//	int row = GaussXX.rows;
//	int col = GaussXX.cols;
//	Mat_<float> dst(row, col, CV_32FC1);
//	for (int i = 0; i < row; i++) {
//		for (int j = 0; j < col; j++) {
//			float a = GaussXX.at<float>(i, j);
//			float b = GaussYY.at<float>(i, j);
//			float c = GaussXY.at<float>(i, j);
//			dst.at<float>(i, j) = a * b - c * c - k * (a + b) * (a + b);
//		}
//	}
//	return dst;
//}

int dir[8][2] = { 0, 1, 0, -1, 1, 0, -1, 0, 1, 1, 1, -1, -1, 1, -1, -1 };

void solve(int, void*) {
	image_my = img.clone();
	int row = image_my.rows;
	int col = image_my.cols;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (C.at<float>(i, j) > threshold1) {
				bool flag = true;
				for (int k = 0; k < 8; k++) {
					int tx = i + dir[k][0];
					int ty = j + dir[k][1];
					if (tx >= 0 && ty >= 0 && tx < row && ty < col && C.at<float>(i, j) < C.at<float>(tx, ty)) {
						flag = false;
						break;
					}
				}
				if (!flag) continue;
				circle(image_my, Point(j, i), 5, Scalar(0, 255, 0), 2, 8, 0);
			}
		}
	}
	imshow("my", image_my);
	cout << "自己写的函数计算耗时：" << duration[0] << "ms" << endl;
}

void solve_opencv(int, void*) {
	image_opencv = img.clone();
	int row = image_opencv.rows;
	int col = image_opencv.cols;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (C_opencv.at<float>(i, j) > threshold2) {
				bool flag = true;
				for (int k = 0; k < 8; k++) {
					int tx = i + dir[k][0];
					int ty = j + dir[k][1];
					if (tx >= 0 && ty >= 0 && tx < row && ty < col && C_opencv.at<float>(i, j) < C_opencv.at<float>(tx, ty)) {
						flag = false;
						break;
					}
				}
				if (!flag) continue;
				circle(image_opencv, Point(j, i), 5, Scalar(0, 255, 0), 2, 8, 0);
			}
		}
	}
	imshow("cornerHarris", image_opencv);
	cout << "opencv的cornerHarris函数计算用时耗时：" << duration[1] << "ms" << endl;
}

int main()
{
	namedWindow("my", WINDOW_NORMAL);
	namedWindow("cornerHarris", WINDOW_NORMAL);
	img = imread("E:/计算机视觉/exp7/2.jpg");
	resize(img, img, Size(300, 300));
	imshow("原始图", img);
	cvtColor(img, image_gray, COLOR_BGR2GRAY);
	//计算dx，dy，dxy
	Begin = clock();
	Sobel(image_gray, dx, CV_32F, 1, 0, 3);
	Sobel(image_gray, dy, CV_32F, 0, 1, 3);
	dxx = dx.mul(dx);
	dxx = dx.mul(dx);
	dyy = dy.mul(dy);
	dxy = dx.mul(dy);

	//对dxx，dyy，dxy进行高斯滤波
	GaussianBlur(dxx, dxx_gauss, Size(7, 7), 0, 0);
	GaussianBlur(dyy, dyy_gauss, Size(7, 7), 0, 0);
	GaussianBlur(dxy, dxy_gauss, Size(7, 7), 0, 0);
	float k = 0.05;
	C = dxx_gauss.mul(dyy_gauss) - dxy_gauss.mul(dxy_gauss) - k * (dxx_gauss + dyy_gauss).mul(dxx_gauss + dyy_gauss);
	End = clock();
	duration[0] = (float)(End - Begin) * 1000 / CLK_TCK;

	//opencv的cornerHarris函数
	Begin = clock();
	cornerHarris(image_gray, C_opencv, 2, 3, k);
	End = clock();
	duration[1] = (float)(End - Begin) * 1000 / CLK_TCK;

	normalize(C, C, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	normalize(C_opencv, C_opencv, 0, 255, NORM_MINMAX, CV_32FC1, Mat());

	int kSize = 2;
	threshold1 = 100;
	threshold2 = 100;
	createTrackbar("阈值", "my", &threshold1, 255, solve);
	createTrackbar("阈值", "cornerHarris", &threshold2, 255, solve_opencv);
	waitKey(0);
	return 0;
}