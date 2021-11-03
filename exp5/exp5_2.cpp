#include <bits/stdc++.h>
#include <opencv2\core\core.hpp> 
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

float eu(int x1, int y1, int x2, int y2)
{
	return sqrt(float((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

//距离函数实现
void mydistance(Mat & src, Mat & dst)
{
	Mat temp = src.clone();
	int rows = src.rows;
	int cols = src.cols;
	float dismin = 0;
	float dis_temp = 0;
	uchar* pDataOne;
	uchar* pDataTwo;
	//第一遍遍历图像，使用左模板
	for (int i = 1; i < rows - 1; i++)
	{
		pDataOne = temp.ptr<uchar>(i);
		for (int j = 1; j < cols; j++)
		{
			pDataTwo = temp.ptr<uchar>(i - 1);
			dis_temp = eu(i, j, i - 1, j - 1);
			dismin = min((float)pDataOne[j], pDataTwo[j - 1] + dis_temp);

			dis_temp = eu(i, j, i - 1, j);
			dismin = min(dismin, pDataTwo[j] + dis_temp);

			pDataTwo = temp.ptr<uchar>(i);
			dis_temp = eu(i, j, i, j - 1);
			dismin = min(dismin, pDataTwo[j - 1] + dis_temp);

			pDataTwo = temp.ptr<uchar>(i + 1);
			dis_temp = eu(i, j, i + 1, j - 1);
			dismin = min(dismin, dis_temp + pDataTwo[j - 1]);
			pDataOne[j] = (uchar)cvRound(dismin);

		}
	}
	//第二遍使用右模板,从右下角开始
	for (int i = rows - 2; i > 0; i--)
	{
		pDataOne = temp.ptr<uchar>(i);
		for (int j = cols - 2; j >= 0; j--)
		{
			pDataTwo = temp.ptr<uchar>(i + 1);
			dis_temp = eu(i, j, i + 1, j);
			dismin = min((float)pDataOne[j], dis_temp + pDataTwo[j]);
			dis_temp = eu(i, j, i + 1, j + 1);
			dismin = min(dismin, pDataTwo[j + 1] + dis_temp);

			pDataTwo = temp.ptr<uchar>(i);
			dis_temp = eu(i, j, i, j + 1);
			dismin = min(dismin, pDataTwo[j + 1] + dis_temp);

			pDataTwo = temp.ptr<uchar>(i - 1);
			dis_temp = eu(i, j, i - 1, j + 1);
			dismin = min(dismin, pDataTwo[j + 1] + dis_temp);
			pDataOne[j] = (uchar)cvRound(dismin);

		}
	}
	dst = temp.clone();
}

int main()
{
	Mat srcImage = imread("E:/计算机视觉/exp5/horse_mask.png");
	//Mat srcImage = imread("E:/计算机视觉/exp5/messi.jpg");
	if (!srcImage.data)
	{
		cout << "读入图片错误！" << endl;
		system("pause");
		return -1;
	}
	//转换为灰度图像 
	Mat grayImage;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	GaussianBlur(grayImage, grayImage, Size(5, 5), 2); //滤波
	//转换为二值图像 
	Mat binaryImage;
	threshold(grayImage, binaryImage, 127, 255, THRESH_BINARY);

	//用于计算图像中每一个非零点（前景）离自己最近的零点（背景）的距离
	Mat image_dis(binaryImage.size(), CV_32FC1); //定义保存距离变换结果的Mat矩阵  
	Mat image_dis_my;
	distanceTransform(binaryImage, image_dis, DIST_L2, DIST_MASK_PRECISE);  //距离变换,使用欧式距离进行计算
	mydistance(binaryImage, image_dis_my);
	Mat dstImage = Mat::zeros(binaryImage.size(), CV_8UC1); 
	for (int i = 0; i < image_dis.rows; i++)
	{
		for (int j = 0; j < image_dis.cols; j++)
		{
			dstImage.at<uchar>(i, j) = image_dis.at<float>(i, j);
		}
	}
	//归一化矩阵 
	normalize(dstImage, dstImage, 0, 255, NORM_MINMAX);
	normalize(image_dis_my, image_dis_my, 0, 1000, NORM_MINMAX);
	imshow("二值化图像", binaryImage);
	imshow("距离变换后的图像", dstImage);
	imshow("mydistance", image_dis_my);
	waitKey();
	return 0;

}