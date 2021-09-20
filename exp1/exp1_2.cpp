#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<cmath>
#include<iostream>
using namespace std;
using namespace cv;

Mat image1, image2, image3;//image1是处理前的原始图像，image2是处理后的图像，image3是可以调整阈值的黑白图像
int mythreshold;//阈值

void sovle(int, void*) {
	double sum;
	for (int x = 0; x < image1.rows; x++) {
		for (int y = 0; y < image1.cols; y++) {
			sum = 0.0;
			for (int z = 0; z < 3; z++) {
				sum += pow((image1.at<Vec3b>(x, y)[z] - image2.at<Vec3b>(x, y)[z]), 2);
			}
			sum = sqrt(sum);

			if (sum >= mythreshold) {
				//如果sum大于阈值，说明这个点是在前景，设置成白色
				for (int z = 0; z < 3; z++) {
					image3.at<Vec3b>(x, y)[z] = 255;
				}
			}
			else {
				//如果sum小于阈值，说明这个点是在背景，设置成黑色
				for (int z = 0; z < 3; z++) {
					image3.at<Vec3b>(x, y)[z] = 0;
				}
			}
		}
	}
	imshow("处理后的图像", image3);
}


int main() {
	//读入文件
	image1 = imread("E:\\计算机视觉\\exp1\\13.jpg");
	image2 = imread("E:\\计算机视觉\\exp1\\13_bg.jpg");
	//绘制两个窗口
	namedWindow("处理后的图像", WINDOW_AUTOSIZE);
	namedWindow("处理前的图像", WINDOW_AUTOSIZE);

	if (!image1.data || !image2.data) {
		cout << "读图失败" << endl;
		return -1;
	}

	mythreshold = 100;
	//初始化image3
	image3 = Mat::zeros(image1.size(), image1.type());
	//画slider
	createTrackbar("背景相减", "处理后的图像", &mythreshold, 200, sovle);
	sovle(mythreshold, 0);
	//画图
	imshow("处理前的图像", image1);
	imshow("背景图", image2);

	waitKey(0);
	return 0;
}