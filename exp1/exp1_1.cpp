//#include <opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include"opencv2/imgproc/imgproc.hpp"
//#include<cmath>
//#include <bits/stdc++.h>
//using namespace std;
//using namespace cv;
//
//
//int contra;//���ƶԱȶ�
//int light;//��������
//Mat image, image0, newimage;
//
////�Զ���������ȺͶԱȶȵĺ���adjustment����Ϊ���������ΪcreateTrackbar�����Ļص��������������Ĳ���������int����void*
//void adjustment(int, void*)
//{
//	for (int x = 0; x < image.rows; x++)
//	{
//		for (int y = 0; y < image.cols; y++)
//		{
//			for (int z = 0; z < 3; z++)
//			{
//				//�ȶ�temp���й�һ�����������ڶԱȶȣ��Աȶ�Խ��temp��ȡֵ��Խ��ɢ
//				double temp = ((image.at<Vec3b>(x, y)[z] - 127.0) / 255.00) * contra * 0.1; 
//				//����õ�temp��ֵ�����µ�ͼ�����ͬ��λ�ã�lightԽ����ֵ��Խ�ߣ�ͼ���Ӧλ�þ�Խ��
//				//saturate_cast�������Ƿ�ֹ��ֵ����������ֵ����255���ͻ᷵��255��С��0�ͻ᷵��0
//				newimage.at<Vec3b>(x, y)[z] = saturate_cast<uchar>(image.at<Vec3b>(x, y)[z] * ((1.00 / (1.00 + exp(-temp))) + 0.4) + light - 100);
//			}
//		}
//	}
//	imshow("exp1_1", newimage);
//}
//int main()
//{
//	image0 = imread("E:/pix/cUNJkLuG.jpg");
//	//���µ���ͼ���С
//	resize(image0, image, Size(512,256), (0, 0), (0, 0), INTER_LINEAR);
//
//	if (!image.data)
//	{
//		cout << "��ȡͼƬʧ��" << endl;
//		return false;
//	}
//	namedWindow("exp1_1", WINDOW_AUTOSIZE);
//	newimage = Mat::zeros(image.size(), image.type());
//
//	contra = 20;
//	light = 100;
//	//�����⵽contra��light�����仯���͵���adjustment�ص�����
//	createTrackbar("Contrast", "exp1_1", &contra, 100, adjustment);
//	createTrackbar("Intensity", "exp1_1", &light, 200, adjustment);
//
//	adjustment(contra, 0);
//	adjustment(light, 0);
//
//	waitKey(0);
//	return 0;
//}