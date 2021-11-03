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

double** getGuassionArray(int size, double sigma) {
	int i, j;
	double sum = 0.0;
	int center = size / 2; //以第一个点的坐标为原点，求出中心点的坐标

	double** arr = new double* [size];//建立一个size*size大小的二维数组
	for (i = 0; i < size; ++i)
		arr[i] = new double[size];

	for (i = 0; i < size; ++i)
		for (j = 0; j < size; ++j) {
			arr[i][j] = exp(-((i - center) * (i - center) + (j - center) * (j - center)) / (sigma * sigma * 2));
			sum += arr[i][j];
		}
	//归一化
	for (i = 0; i < size; ++i)
		for (j = 0; j < size; ++j)
			arr[i][j] /= sum;
	return arr;
}

double* getGuassionArray_XY(int size, double sigma) {
	int i;
	double sum = 0.0;
	int center = size / 2; //以第一个点的坐标为原点，求出中心点的坐标

	double* arr = new double[size];//建立一个size*size大小的二维数组

	for (i = 0; i < size; ++i) {
		arr[i] = exp(-((i - center) * (i - center)) / (sigma * sigma * 2));
		sum += arr[i];
	}

	//归一化
	for (i = 0; i < size; ++i)
		arr[i] /= sum;
	return arr;
}

void myGaussian(const Mat _src, Mat & _dst, int k_size, int sigma) {
	if (!_src.data) return;
	double** arr;
	arr = getGuassionArray(k_size, sigma);//自定义得到的权值数组
	int pad = k_size / 2;
	Mat src_board;
	copyMakeBorder(_src, src_board, pad, pad, pad, pad, BORDER_REFLECT);   //扩充边缘
	_dst = Mat::zeros(_src.size(), _src.type());
	for (int i = pad; i < src_board.rows - pad; i++)
		for (int j = pad; j < src_board.cols - pad; j++) {
			for (int x = 0; x < k_size; x++) {
				for (int y = 0; y < k_size; y++) {
					_dst.at<Vec3b>(i - pad, j - pad)[0] += arr[x][y] * src_board.at<Vec3b>(i + pad - x, j + pad - y)[0];
					_dst.at<Vec3b>(i - pad, j - pad)[1] += arr[x][y] * src_board.at<Vec3b>(i + pad - x, j + pad - y)[1];
					_dst.at<Vec3b>(i - pad, j - pad)[2] += arr[x][y] * src_board.at<Vec3b>(i + pad - x, j + pad - y)[2];
				}
			}
		}
}

void myGaussian_XY(const Mat _src, Mat & _dst, int k_size, int sigma) {
	if (!_src.data) return;
	double* arr;
	arr = getGuassionArray_XY(k_size, sigma);//自定义得到的权值数组
	int pad = k_size / 2;
	Mat src_board;
	//边缘填充
	copyMakeBorder(_src, src_board, pad, pad, pad, pad, BORDER_REFLECT);   //扩充边缘
	Mat temp = Mat::zeros(src_board.size(), _src.type());
	_dst = Mat::zeros(_src.size(), _src.type());

	//x方向一维卷积
	for (int i = 0; i < src_board.rows; i++) {
		for (int j = pad; j < src_board.cols - pad; j++) {
			for (int x = 0; x < k_size; x++) {
				temp.at<Vec3b>(i, j)[0] += arr[x] * src_board.at<Vec3b>(i, j + pad - x)[0];
				temp.at<Vec3b>(i, j)[1] += arr[x] * src_board.at<Vec3b>(i, j + pad - x)[1];
				temp.at<Vec3b>(i, j)[2] += arr[x] * src_board.at<Vec3b>(i, j + pad - x)[2];
			}
		}
	}
	//y方向一维卷积
	for (int i = pad; i < src_board.rows - pad; i++) {
		for (int j = pad; j < src_board.cols - pad; j++) {
			for (int y = 0; y < k_size; y++) {
				_dst.at<Vec3b>(i - pad, j - pad)[0] += arr[y] * temp.at<Vec3b>(i + pad - y, j)[0];
				_dst.at<Vec3b>(i - pad, j - pad)[1] += arr[y] * temp.at<Vec3b>(i + pad - y, j)[1];
				_dst.at<Vec3b>(i - pad, j - pad)[2] += arr[y] * temp.at<Vec3b>(i + pad - y, j)[2];
			}
		}
	}
}

int main() {
	int k_size = 3, sigma = 1;
	cout << "请输入sigma的值" << endl;
	cin >> sigma;
	k_size = int(6 * sigma - 1);
	Mat image = imread("E:/pix/messi.jpg");
	resize(image, image, Size(400, 400), (0, 0), (0, 0), INTER_LINEAR);
	Mat _gaussian, _gaussian_xy;
	Mat image1;
	Begin = clock();
	myGaussian(image, _gaussian, k_size, sigma);
	End = clock();
	duration[0] = (float)(End - Begin) * 1000 / CLK_TCK;
	Begin = clock();
	myGaussian_XY(image, _gaussian_xy, k_size, sigma);
	End = clock();
	duration[1] = (float)(End - Begin) * 1000 / CLK_TCK;
	GaussianBlur(image, image1, Size(k_size, k_size), sigma);

	imshow("原图", image);
	imshow("opencv自带的高斯滤波", image1);
	imshow("自定义高斯滤波", _gaussian);
	imshow("行列分离的高斯滤波", _gaussian_xy);
	cout << "在sigma = " << sigma << "的情况下" << "\n" << "二维高斯滤波执行的时间为：" << duration[0] << "ms\n" << "一维高斯滤波执行的时间为：" << duration[1] << "ms" << endl;
	waitKey();
	return 0;
}