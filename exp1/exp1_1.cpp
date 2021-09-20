//#include <opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include"opencv2/imgproc/imgproc.hpp"
//#include<cmath>
//#include <bits/stdc++.h>
//using namespace std;
//using namespace cv;
//
//
//int contra;//控制对比度
//int light;//控制亮度
//Mat image, image0, newimage;
//
////自定义调节亮度和对比度的函数adjustment，因为这个函数作为createTrackbar函数的回调函数，所以它的参数必须是int，和void*
//void adjustment(int, void*)
//{
//	for (int x = 0; x < image.rows; x++)
//	{
//		for (int y = 0; y < image.cols; y++)
//		{
//			for (int z = 0; z < 3; z++)
//			{
//				//先对temp进行归一化操作，调节对比度，对比度越大，temp的取值就越分散
//				double temp = ((image.at<Vec3b>(x, y)[z] - 127.0) / 255.00) * contra * 0.1; 
//				//将算好的temp的值赋给新的图像的相同的位置，light越大，数值就越高，图像对应位置就越亮
//				//saturate_cast的作用是防止数值溢出，如果数值大于255，就会返回255，小于0就会返回0
//				newimage.at<Vec3b>(x, y)[z] = saturate_cast<uchar>(image.at<Vec3b>(x, y)[z] * ((1.00 / (1.00 + exp(-temp))) + 0.4) + light - 100);
//			}
//		}
//	}
//	imshow("exp1_1", newimage);
//}
//int main()
//{
//	image0 = imread("E:/pix/cUNJkLuG.jpg");
//	//重新调整图像大小
//	resize(image0, image, Size(512,256), (0, 0), (0, 0), INTER_LINEAR);
//
//	if (!image.data)
//	{
//		cout << "读取图片失败" << endl;
//		return false;
//	}
//	namedWindow("exp1_1", WINDOW_AUTOSIZE);
//	newimage = Mat::zeros(image.size(), image.type());
//
//	contra = 20;
//	light = 100;
//	//如果检测到contra或light发生变化，就调用adjustment回调函数
//	createTrackbar("Contrast", "exp1_1", &contra, 100, adjustment);
//	createTrackbar("Intensity", "exp1_1", &light, 200, adjustment);
//
//	adjustment(contra, 0);
//	adjustment(light, 0);
//
//	waitKey(0);
//	return 0;
//}