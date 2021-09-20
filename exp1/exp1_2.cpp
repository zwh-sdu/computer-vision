#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<cmath>
#include<iostream>
using namespace std;
using namespace cv;

Mat image1, image2, image3;//image1�Ǵ���ǰ��ԭʼͼ��image2�Ǵ�����ͼ��image3�ǿ��Ե�����ֵ�ĺڰ�ͼ��
int mythreshold;//��ֵ

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
				//���sum������ֵ��˵�����������ǰ�������óɰ�ɫ
				for (int z = 0; z < 3; z++) {
					image3.at<Vec3b>(x, y)[z] = 255;
				}
			}
			else {
				//���sumС����ֵ��˵����������ڱ��������óɺ�ɫ
				for (int z = 0; z < 3; z++) {
					image3.at<Vec3b>(x, y)[z] = 0;
				}
			}
		}
	}
	imshow("������ͼ��", image3);
}


int main() {
	//�����ļ�
	image1 = imread("E:\\������Ӿ�\\exp1\\13.jpg");
	image2 = imread("E:\\������Ӿ�\\exp1\\13_bg.jpg");
	//������������
	namedWindow("������ͼ��", WINDOW_AUTOSIZE);
	namedWindow("����ǰ��ͼ��", WINDOW_AUTOSIZE);

	if (!image1.data || !image2.data) {
		cout << "��ͼʧ��" << endl;
		return -1;
	}

	mythreshold = 100;
	//��ʼ��image3
	image3 = Mat::zeros(image1.size(), image1.type());
	//��slider
	createTrackbar("�������", "������ͼ��", &mythreshold, 200, sovle);
	sovle(mythreshold, 0);
	//��ͼ
	imshow("����ǰ��ͼ��", image1);
	imshow("����ͼ", image2);

	waitKey(0);
	return 0;
}