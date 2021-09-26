#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

Mat solve(Mat frame) {
	Mat frame_after(frame.rows, frame.cols, CV_8UC3);
	int row = frame_after.rows;
	int col = frame_after.cols;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			//中心归一化坐标
			double x_normal = i / (0.5 * row) - 1;
			double y_normal = j / (0.5 * col) - 1;
			double r = sqrt(pow(x_normal, 2.0) + pow(y_normal, 2.0));
			double angle = pow(1 - r, 2.0);
			double x_normal_after;
			double y_normal_after;
			if (r >= 1)
			{
				x_normal_after = x_normal;
				y_normal_after = y_normal;
			}
			else {
				x_normal_after = cos(angle) * x_normal + sin(angle) * y_normal;
				y_normal_after = sin(angle) * x_normal - cos(angle) * y_normal;
			}
			//将中心归一化的坐标还原
			int x_after = (x_normal_after + 1) * 0.5 * row;
			int y_after = (y_normal_after + 1) * 0.5 * col;
			frame_after.ptr(i, j)[0] = frame.ptr(x_after, y_after)[0];
			frame_after.ptr(i, j)[1] = frame.ptr(x_after, y_after)[1];
			frame_after.ptr(i, j)[2] = frame.ptr(x_after, y_after)[2];
		}
	}
	return frame_after;
}

Mat solve1(Mat frame) {
	Mat frame_after(frame.rows, frame.cols, CV_8UC3);
	int row = frame_after.rows;
	int col = frame_after.cols;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			//中心归一化坐标
			double x_normal = i / (0.5 * row) - 1;
			double y_normal = j / (0.5 * col) - 1;
			double r = 0.8;
			double x_normal_after;
			double y_normal_after;
			//变换后的新坐标
			x_normal_after = (x_normal / 2) * (sqrt(x_normal * x_normal + y_normal * y_normal) / r);
			y_normal_after = (y_normal / 2) * (sqrt(y_normal * y_normal + x_normal * x_normal) / r);

			//将中心归一化的坐标还原
			int x_after = (x_normal_after + 1) * 0.5 * row;
			int y_after = (y_normal_after + 1) * 0.5 * col;
			frame_after.ptr(i, j)[0] = frame.ptr(x_after, y_after)[0];
			frame_after.ptr(i, j)[1] = frame.ptr(x_after, y_after)[1];
			frame_after.ptr(i, j)[2] = frame.ptr(x_after, y_after)[2];
		}
	}
	return frame_after;
}

Mat bilinearpolation(Mat& src) {
	int row = src.rows;
	int col = src.cols;
	Mat dst(row, col, CV_8UC3);
	for (int i = 0; i < row; i++) {
		double x_normal = i / (0.5 * row) - 1;
		double x_normal_after;
		for (int j = 0; j < col; j++) {
			double y_normal = j / (0.5 * col) - 1;
			double y_normal_after;
			x_normal_after = (x_normal / 2) * (sqrt(x_normal * x_normal + y_normal * y_normal));
			y_normal_after = (y_normal / 2) * (sqrt(y_normal * y_normal + x_normal * x_normal));
			//将中心归一化的坐标还原
			double x_after = (x_normal_after + 1) * 0.5 * row;
			double y_after = (y_normal_after + 1) * 0.5 * col;
			if (x_after < 0) x_after = 0;
			if (x_after > row - 1) x_after = row - 1;
			if (y_after < 0) y_after = 0;
			if (y_after > col - 1) y_after = col - 1;
			//双线性插值
			int i1 = cvFloor(x_after);
			int i2 = cvCeil(x_after);
			int j1 = cvFloor(y_after);
			int j2 = cvCeil(y_after);
			double v = x_after - i1;
			double u = y_after - j1;
			dst.at<Vec3b>(i, j)[0] = cvFloor((1 - u) * (1 - v) * src.at<Vec3b>(i1, j1)[0] + (1 - u) * v * src.at<Vec3b>(i2, j1)[0] + u * (1 - v) * src.at<Vec3b>(i1, j2)[0] + u * v * src.at<Vec3b>(i2, j2)[0]);
			dst.at<Vec3b>(i, j)[1] = cvFloor((1 - u) * (1 - v) * src.at<Vec3b>(i1, j1)[1] + (1 - u) * v * src.at<Vec3b>(i2, j1)[1] + u * (1 - v) * src.at<Vec3b>(i1, j2)[1] + u * v * src.at<Vec3b>(i2, j2)[1]);
			dst.at<Vec3b>(i, j)[2] = cvFloor((1 - u) * (1 - v) * src.at<Vec3b>(i1, j1)[2] + (1 - u) * v * src.at<Vec3b>(i2, j1)[2] + u * (1 - v) * src.at<Vec3b>(i1, j2)[2] + u * v * src.at<Vec3b>(i2, j2)[2]);
			
			////优化算法减少乘法次数
			//for (int x = 0; x < 3; x++) {
			//	double h1 = src.at<Vec3b>(i1, j1)[x]*1.0 + 1.0*v * (src.at<Vec3b>(i2, j1)[x]*1.0 - 1.0*src.at<Vec3b>(i1, j1)[x]);
			//	double h2 = src.at<Vec3b>(i1, j2)[x]*1.0 + 1.0*v * (src.at<Vec3b>(i2, j2)[x]*1.0 - 1.0*src.at<Vec3b>(i2, j1)[x]);
			//	dst.at<Vec3b>(i, j)[x] = cvFloor(h1 + u * (h2 - h1));
			//}
		}
	}
	return dst;
}

int main()
{
	VideoCapture cap;
	//VideoWriter writer;
	//int codec = writer.fourcc('M', 'J', 'P', 'G');
	//writer = VideoWriter("E:\\计算机视觉\\exp2\\哈哈镜效果.avi", codec, 10, cv::Size(640, 480));
	cap.open(0);
	while (1)
	{
		Mat frame;
		cap >> frame;
		if (frame.empty()) {
			cout << "finish" << endl;
			break;
		}
		Mat framet = bilinearpolation(frame);
		//writer << framet;
		imshow("input video", solve1(frame));
		imshow("input video1", framet);
		waitKey(1);
	}
	//writer.release();
	cap.release();
	return 0;
}