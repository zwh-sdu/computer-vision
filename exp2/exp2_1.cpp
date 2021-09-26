//#include<opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include<iostream>
//#include<math.h>
//#include<imgproc.hpp>
//
//using namespace cv;
//using namespace std;
//
//Mat bilinearpolation(Mat& src) {
//	int row = src.rows;
//	int col = src.cols;
//	Mat dst = Mat::zeros(src.size(), src.type());
//	for (int i = 0; i < row; i++) {
//		for (int j = 0; j < col; j++) {
//			double x_normal = i / (0.5 * row) - 1;
//			double y_normal = j / (0.5 * col) - 1;
//			double r = sqrt(pow(x_normal, 2.0) + pow(y_normal, 2.0));
//			double angle = pow(1 - r, 2.0);
//			double x_normal_before;
//			double y_normal_before;
//			if (r >= 1)
//			{
//				x_normal_before = x_normal;
//				y_normal_before = y_normal;
//			}
//			else {
//				x_normal_before = cos(angle) * x_normal - sin(angle) * y_normal;
//				y_normal_before = sin(angle) * x_normal + cos(angle) * y_normal;
//			}
//			//将中心归一化的坐标还原
//			double x_before = (x_normal_before + 1) * 0.5 * row;
//			double y_before = (y_normal_before + 1) * 0.5 * col;
//			if (x_before < 0) x_before = 0;
//			if (x_before > row - 1) x_before = row - 1;
//			if (y_before < 0) y_before = 0;
//			if (y_before > col - 1) y_before = col - 1;
//			//双线性插值
//			int i1 = cvFloor(x_before);
//			int i2 = cvCeil(x_before);
//			int j1 = cvFloor(y_before);
//			int j2 = cvCeil(y_before);
//			double v = x_before - i1;
//			double u = y_before - j1;
//			dst.at<Vec3b>(i, j)[0] = cvFloor((1 - u) * (1 - v) * src.at<Vec3b>(i1, j1)[0] + (1 - u) * v * src.at<Vec3b>(i2, j1)[0] + u * (1 - v) * src.at<Vec3b>(i1, j2)[0] + u * v * src.at<Vec3b>(i2, j2)[0]);
//			dst.at<Vec3b>(i, j)[1] = cvFloor((1 - u) * (1 - v) * src.at<Vec3b>(i1, j1)[1] + (1 - u) * v * src.at<Vec3b>(i2, j1)[1] + u * (1 - v) * src.at<Vec3b>(i1, j2)[1] + u * v * src.at<Vec3b>(i2, j2)[1]);
//			dst.at<Vec3b>(i, j)[2] = cvFloor((1 - u) * (1 - v) * src.at<Vec3b>(i1, j1)[2] + (1 - u) * v * src.at<Vec3b>(i2, j1)[2] + u * (1 - v) * src.at<Vec3b>(i1, j2)[2] + u * v * src.at<Vec3b>(i2, j2)[2]);
//		}
//	}
//	return dst;
//}
//
//int main()
//{
//	Mat image_before = imread("E:/计算机视觉/exp2/1.jpg");
//	//Mat image_before;
//	//我找的这个图片太大了，先缩小一点
//	//resize(imagebig, image_before, Size(512, 512), (0, 0), (0, 0), INTER_LINEAR);
//	Mat image_after = Mat::zeros(image_before.size(), image_before.type());
//	int row = image_after.rows;
//	int col = image_after.cols;
//	for (int i = 0; i < row; i++)
//	{
//		for (int j = 0; j < col; j++)
//		{
//			//中心归一化坐标
//			double x_normal = i / (0.5 * row) - 1;
//			double y_normal = j / (0.5 * col) - 1;
//			double r = sqrt(pow(x_normal, 2.0) + pow(y_normal, 2.0));
//			double angle = pow(1 - r, 2.0);
//			double x_normal_after;
//			double y_normal_after;
//			if (r >= 1)
//			{
//				x_normal_after = x_normal;
//				y_normal_after = y_normal;
//			}
//			else {
//				x_normal_after = cos(angle) * x_normal - sin(angle) * y_normal;
//				y_normal_after = sin(angle) * x_normal + cos(angle) * y_normal;
//			}
//			//将中心归一化的坐标还原
//			int x_after = (x_normal_after + 1) * 0.5 * row;
//			int y_after = (y_normal_after + 1) * 0.5 * col;
//			image_after.at<Vec3b>(i, j)[0] = image_before.at<Vec3b>(x_after, y_after)[0];
//			image_after.at<Vec3b>(i, j)[1] = image_before.at<Vec3b>(x_after, y_after)[1];
//			image_after.at<Vec3b>(i, j)[2] = image_before.at<Vec3b>(x_after, y_after)[2];
//		}
//	}
//	imshow("原图", image_before);
//	imshow("变形后", image_after);
//	imshow("双线性插值", bilinearpolation(image_before));
//	waitKey();
//}