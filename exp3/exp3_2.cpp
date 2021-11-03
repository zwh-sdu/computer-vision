#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<math.h>
#include<ctime>
#include<imgproc.hpp>
#define CLK_TCK  CLOCKS_PER_SEC //机器时钟每秒的打点数
clock_t Begin, End;
double duration[2];
using namespace cv;
using namespace std;

void meanfilter_fast(const Mat _src, Mat& _dst, int w)
{
	//copyMakeBorder(temp, temp, 1, 1, 1, 1, BORDER_REFLECT);   //扩充边缘

	int*** a = new int * *[_src.rows+1];
	for (int i = 0; i < _src.rows+1; i++) {
		a[i] = new int* [_src.cols+1];
	}
	for (int i = 0; i < _src.rows+1; i++) {
		for (int j = 0; j < _src.cols+1; j++) {
			a[i][j] = new int[3];
		}
	}
	for (int i = 1; i < _src.rows + 1; i++) {
		for (int j = 1; j < _src.cols + 1; j++) {
			for (int k = 0; k < 3; k++) {
				a[i][j][k] = a[i][j - 1][k] + a[i - 1][j][k] - a[i - 1][j - 1][k] + (int)_src.at<Vec3b>(i, j)[k];
			}
		}
	}
	_dst = Mat::zeros(_src.size(), _src.type());
	for (int i = 1; i < _src.rows + 1; i++) {
		for (int j = 1; j < _src.cols + 1; j++) {
			int y1 = max(j - w, 1), y2 = min(j + w, _src.cols);
			int x1 = max(i - w, 1), x2 = min(i + w, _src.rows);

			for (int k = 0; k < 3; k++) {
				_dst.at<Vec3b>(i-1, j-1)[k] = (a[x2][y2][k] - a[x2][y1 - 1][k] - a[x1 - 1][y2][k] + a[x1 - 1][y1 - 1][k]) / ((x2 - x1 + 1) * (y2 - y1 + 1));
			}
		}
	}
}

//图像椒盐化
void salt(Mat & image, int num) {
	if (!image.data) return;//防止传入空图
	int i, j;
	srand(time(NULL));
	for (int x = 0; x < num; ++x) {
		i = rand() % image.rows;
		j = rand() % image.cols;
		image.at<Vec3b>(i, j)[0] = 255;
		image.at<Vec3b>(i, j)[1] = 255;
		image.at<Vec3b>(i, j)[2] = 255;
	}
}

int main() {
	int w;
	cout << "请输入w的值" << endl;
	cin >> w;
	Mat image = imread("E:/pix/messi.jpg");
	Mat image1, image2, saltimage;
	resize(image, image, Size(400, 400), (0, 0), (0, 0), INTER_LINEAR);
	image.copyTo(saltimage);
	salt(saltimage, 3000);
	Begin = clock();
	meanfilter_fast(saltimage, image1, w);
	End = clock();
	duration[0] = (float)(End - Begin) * 1000 / CLK_TCK;
	Begin = clock();
	boxFilter(saltimage, image2,-1,Size(2 * w + 1, 2 * w + 1));//openCV库自带的boxFilter滤波函数
	End = clock();
	duration[1] = (float)(End - Begin) * 1000 / CLK_TCK;
	imshow("原图", image);
	imshow("椒盐化后的图", saltimage);
	imshow("快速均值滤波后的图", image1);
	imshow("opencv自带的均值滤波后的图", image2);
	cout << "在w = " << w << "的情况下" << "\n" << "自定义的快速均值滤波执行的时间为：" << duration[0] << "ms\n" << "opencv中boxFilter滤波执行的时间为：" << duration[1] << "ms" << endl;
	waitKey();
	return 0;
}