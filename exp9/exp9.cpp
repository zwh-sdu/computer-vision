#include<opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "../../../opencv/build/include/opencv2/imgproc/types_c.h"
#include<opencv2/core/core.hpp>
#include<bits/stdc++.h>
using namespace std;
using namespace cv;

int main()
{
	//����Դͼ���ģ��ͼ��
	Mat image_source = imread("E:/������Ӿ�/exp9/messi.jpg");
	Mat image_gray;
	imshow("ԭͼ��", image_source);
	cvtColor(image_source, image_gray, COLOR_BGR2GRAY);
	Mat image_template = imread("E:/������Ӿ�/exp9/10_dark.jpg");
	imshow("ģ��ͼ��", image_template);
	cvtColor(image_template, image_template, COLOR_BGR2GRAY);

	//ģ��ƥ��
	Mat image_matched;
	matchTemplate(image_gray, image_template, image_matched, TM_SQDIFF);

	//Ѱ�����ƥ��λ��
	double minVal, maxVal;
	Point minLoc, maxLoc;
	minMaxLoc(image_matched, &minVal, &maxVal, &minLoc, &maxLoc);

	circle(image_source,
		Point(minLoc.x + image_template.cols / 2, minLoc.y + image_template.rows / 2),
		40,
		Scalar(0, 0, 255),
		2,
		8,
		0);

	imshow("match result", image_matched);
	imshow("target", image_source);
	waitKey(0);

	return 0;
}