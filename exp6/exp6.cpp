#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<bits/stdc++.h>

using namespace std;
using namespace cv;

#define CLK_TCK  CLOCKS_PER_SEC //机器时钟每秒的打点数
clock_t Begin, End;
double duration[3];

Mat img, image_edge, image_gray, edge1, dst, dst1, dx, dy;
int threshold1, threshold2, minDist, minr, maxr;
int H[800][800][100], vis[800][800][100];

static void Hough(int, void*)
{
	Begin = clock();
	cout << "start" << endl;
	img.copyTo(dst);
	minr = 10;
	maxr = 100;
	//vector<vector<vector<int>>> H(image_edge.rows, vector<vector<int>>(image_edge.cols, vector<int>(100, 0)));
	//vector<vector<vector<int>>> vis(image_edge.rows, vector<vector<int>>(image_edge.cols, vector<int>(100, 0)));
	memset(H, 0, sizeof(H));
	memset(vis, 0, sizeof(vis));
	int a, b;
	double theta;
	for (int i = 0; i < image_edge.rows; i++) {
		for (int j = 0; j < image_edge.cols; j++) {
			if ((int)image_edge.ptr<uchar>(i)[j] > 0) {
				//circle(dst, Point(j, i), 3, Scalar(0, 255, 0), -1, 8, 0);
				for (int r = minr; r < maxr; r++) {
					for (int t = 0; t < 618; t++) {
						theta = 1.0 * t / 100.0;
						a = i + r * cos(theta);
						b = j + r * sin(theta);
						if (a >= 0 && a < image_edge.rows && b >= 0 && b < image_edge.cols)
							H[a][b][r]++;
					}
				}
			}
		}
	}
	cout << "mid" << endl;
	minDist = 20;
	for (int i = 0; i < image_edge.rows; i++) {
		for (int j = 0; j < image_edge.cols; j++) {
			for (int r = minr; r < maxr; r++) {
				if (H[i][j][r] >= threshold1 && !vis[i][j][r]) {
					circle(dst, Point(j, i), 3, Scalar(0, 255, 0), -1, 8, 0);
					circle(dst, Point(j, i), r, Scalar(155, 50, 255), 2, 8, 0);
					//为了避免画出非常近的圆，当前和这个圆画出来之后，维护vis数组，表明以(x0,y0)，r0为半径的圆之后就不要再画了
					for (int x0 = max(0, i - minDist); x0 < min(image_edge.rows, i + minDist); x0++) {
						for (int y0 = max(0, j - minDist); y0 < min(image_edge.cols, j + minDist); y0++) {
							for (int r0 = max(minr, r - 10); r0 < min(maxr, r + 10); r0++) {
								vis[x0][y0][r0] = 1;
							}
						}
					}
				}
			}
		}
	}
	cout << "finish" << endl;
	End = clock();
	duration[0] = (float)(End - Begin) * 1000 / CLK_TCK;
	cout << "myHough算法用时：" << duration[0] << "ms" << endl;
	imshow("Hough", dst);
}

static void Hough_fast(int, void*)
{
	Begin = clock();
	cout << "start" << endl;
	img.copyTo(dst);
	minr = 10;
	maxr = 100;
	//vector<vector<vector<int>>> H(image_edge.rows, vector<vector<int>>(image_edge.cols, vector<int>(100, 0)));
	//vector<vector<vector<int>>> vis(image_edge.rows, vector<vector<int>>(image_edge.cols, vector<int>(100, 0)));
	memset(H, 0, sizeof(H));
	memset(vis, 0, sizeof(vis));
	int a, b;
	for (int i = 0; i < image_edge.rows; i++) {
		for (int j = 0; j < image_edge.cols; j++) {
			if ((int)image_edge.ptr<uchar>(i)[j] > 0) {
				//circle(dst, Point(j, i), 3, Scalar(0, 255, 0), -1, 8, 0);
				for (int r = minr; r < maxr; r++) {
					for (double theta = -0.5; theta <= 0.5; theta = theta + 0.05) {
						a = int(i + r * cos(atan(dy.ptr<float>(i)[j] / dx.ptr<float>(i)[j]) + theta));
						b = int(j + r * sin(atan(dy.ptr<float>(i)[j] / dx.ptr<float>(i)[j]) + theta));
						if (a >= 0 && a < image_edge.rows && b >= 0 && b < image_edge.cols) {
							H[a][b][r] += 1;
						}
					}
				}
			}
		}
	}
	cout << "mid" << endl;
	minDist = 20;
	for (int i = 0; i < image_edge.rows; i++) {
		for (int j = 0; j < image_edge.cols; j++) {
			for (int r = minr; r < maxr; r++) {
				if (H[i][j][r] >= threshold2 && !vis[i][j][r]) {
					circle(dst, Point(j, i), 3, Scalar(0, 255, 0), -1, 8, 0);
					circle(dst, Point(j, i), r, Scalar(155, 50, 255), 2, 8, 0);
					for (int x0 = max(0, i - minDist); x0 < min(image_edge.rows, i + minDist); x0++) {
						for (int y0 = max(0, j - minDist); y0 < min(image_edge.cols, j + minDist); y0++) {
							for (int r0 = max(minr, r - 10); r0 < min(maxr, r + 10); r0++) {
								vis[x0][y0][r0] = 1;
							}
						}
					}
				}
			}
		}
	}
	cout << "finish" << endl;
	End = clock();
	duration[2] = (float)(End - Begin) * 1000 / CLK_TCK;
	cout << "myHough_fast算法用时：" << duration[2] << "ms" << endl;
	imshow("Hough_fast", dst);
}

int main()
{
	img = imread("E:/计算机视觉/exp6/Natural Image Dataset/3.jpg");
	imshow("原始图", img);
	namedWindow("Hough", WINDOW_NORMAL);
	namedWindow("Hough_fast", WINDOW_NORMAL);

	//将原始图转化为灰度图
	cvtColor(img, image_gray, COLOR_BGR2GRAY);
	cout << image_gray.rows << " " << image_gray.cols << endl;
	//计算dx，dy
	Sobel(image_gray, dx, CV_32F, 1, 0, 3);
	Sobel(image_gray, dy, CV_32F, 0, 1, 3);
	//for (int i = 0; i < image_gray.rows; i++) {
	//	for (int j = 0; j < image_gray.cols; j++) {
	//		cout<<int(dx.ptr<float>(i)[j])<<" ";
	//	}
	//	cout << endl;
	//}
	//cout << "&&&&" << endl;
	//先使用5*5内核来降噪
	imshow("降噪前", image_gray);
	blur(image_gray, image_gray, Size(5, 5));
	imshow("降噪后", image_gray);

	//for (int i = 0; i < 10; i++) {
	//	for (int j = 0; j < 10; j++) {
	//		cout << (int)image_gray.ptr<uchar>(i)[j] << " ";
	//	}
	//	cout << endl;
	//}

	//运行canny算子
	Canny(image_gray, image_edge, 70, 100, 3);
	imshow("边缘检测", image_edge);


	threshold1 = 300;
	createTrackbar("threshold", "Hough", &threshold1, 400, Hough);
	threshold2 = 20;
	createTrackbar("threshold", "Hough_fast", &threshold2, 40, Hough_fast);

	//opencv自带的函数
	vector<Vec3f> circles;
	//霍夫圆
	Begin = clock();
	img.copyTo(dst1);
	HoughCircles(image_gray, circles, HOUGH_GRADIENT, 1, 60, 150, 70, 0, 0);
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		//绘制圆心  
		circle(dst1, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		//绘制圆轮廓  
		circle(dst1, center, radius, Scalar(155, 50, 255), 2, 8, 0);
	}
	imshow("HoughCircles", dst1);
	End = clock();
	duration[1] = (float)(End - Begin) * 1000 / CLK_TCK;
	cout << "opencv的HoughCircles函数用时：" << duration[1] << "ms" << endl;
	waitKey(0);
	return 0;
}