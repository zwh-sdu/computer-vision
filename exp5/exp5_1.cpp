#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<bits/stdc++.h>
#define CLK_TCK  CLOCKS_PER_SEC //机器时钟每秒的打点数
clock_t Begin, End;
double duration[2];

using namespace cv;
using namespace std;

int maxlabel, label_max;	//有多少个连通域

void two_pass(const Mat& src, Mat& dst)
{
	if (src.empty() || src.type() != CV_8UC1)
	{
		return;
	}
	//第一遍扫描
	dst.release();
	src.convertTo(dst, CV_32SC1);	//转换成有符号整形，单通道模式

	int label = 1;
	vector<int> label_equal;//记录标签的集合。 例如label_equal[a]=b意思是label=a和label=b是等价的
	label_equal.push_back(0);
	label_equal.push_back(1);

	int rows = src.rows - 1;
	int cols = src.cols - 1;

	//单独处理第一行
	int* curline = dst.ptr<int>(0);
	for (int j = 0; j <= cols; j++)
	{
		if (curline[j] == 1)
		{
			if (j - 1 >= 0 && curline[j - 1] > 1)
			{
				curline[j] = label;
			}
			else		//如果当前像素的邻域还没有打过标签，说明当前像素一定是一个新的连通域。
			{
				label_equal.push_back(++label);//给当前像素打上一个新的标签
				curline[j] = label;
			}
		}
	}
	for (int i = 1; i <= rows; i++)
	{
		int* preline = dst.ptr<int>(i - 1);
		int* curline = dst.ptr<int>(i);
		for (int j = 0; j <= cols; j++)
		{
			if (curline[j] == 1)
			{
				vector<int> neighborlabel;

				//因为遍历是先按行顺序再按列顺序遍历的，所以这四个邻域像素一定已经被遍历过了
				//如果像素大于1说明之前被标记过，说明是前景，否则是背景，忽略即可。
				if (j - 1 >= 0 && preline[j - 1] > 1)
				{
					neighborlabel.push_back(preline[j - 1]);
				}
				if (preline[j] > 1)
				{
					neighborlabel.push_back(preline[j]);
				}
				if (j + 1 <= cols && preline[j + 1] > 1)
				{
					neighborlabel.push_back(preline[j + 1]);
				}
				if (j - 1 >= 0 && curline[j - 1] > 1)
				{
					neighborlabel.push_back(curline[j - 1]);
				}

				if (neighborlabel.empty())		//如果当前像素的邻域还没有打过标签，说明当前像素一定是一个新的连通域。
				{
					label_equal.push_back(++label);//给当前像素打上一个新的标签
					curline[j] = label;
				}
				else    //如果发现当前像素的邻域已经打过标签了
				{
					//找到当前邻域中最小的label
					int label_min = *min_element(neighborlabel.begin(), neighborlabel.end());
					curline[j] = label_min;			//给当前像素位置打上邻域中最小的label
					//存储两个等价的label值，方便second pass时更新label
					for (int k = 0; k < neighborlabel.size(); k++)
					{
						int label_neighbor = neighborlabel[k];
						int smaller = label_equal[label_neighbor];	//看看有没有与邻域label等价的更小的等价类
						if (smaller > label_min)
						{
							label_equal[smaller] = label_min;	//表示smaller和label_min是等价的
							label_equal[label_neighbor] = label_min;
						}
						else if (smaller < label_min)
						{
							label_equal[label_min] = smaller;	//表示smaller和label_min是等价的
						}
					}
				}
			}
		}
	}
	vector<int> label_sum(label + 1, 0);
	set<int> label_set;
	//将等价的label都变成最小的label
	for (int i = 2; i < label_equal.size(); i++)
	{
		int curLabel = label_equal[i];
		int preLabel = label_equal[curLabel];
		while (preLabel != curLabel)//说明有与curLable等价且比curLabel更小的label
		{
			curLabel = preLabel;
			preLabel = label_equal[preLabel];
		}
		label_sum[curLabel]++;
		label_set.insert(curLabel);
		label_equal[i] = curLabel;
	}
	//获得一共有多少个连通域
	maxlabel = label_set.size();
	//获得哪个label数量最多
	label_max = max_element(label_sum.begin(), label_sum.end()) - label_sum.begin();

	//second pass更新所有等价的label为其中最小的label
	for (int i = 0; i < rows; i++)
	{
		int* data = dst.ptr<int>(i);
		for (int j = 0; j < cols; j++)
		{
			data[j] = label_equal[data[j]];
		}
	}
}

void seedfill(const Mat & src, Mat & dst)
{
	if (src.empty() || src.type() != CV_8UC1)
	{
		return;
	}
	//第一遍扫描
	dst.release();
	src.convertTo(dst, CV_32SC1);	//转换成有符号整形，单通道模式

	int label = 1;

	int rows = src.rows - 1;
	int cols = src.cols - 1;
	for (int i = 0; i <= rows; i++)
	{
		int* data = dst.ptr<int>(i);
		for (int j = 0; j <= cols; j++)
		{
			if (data[j] == 1)
			{
				stack<pair<int, int>> neighborlabel;
				neighborlabel.push(make_pair(i, j));
				label++;
				//maxlabel = label - 1;
				//cout << label << endl;
				while (!neighborlabel.empty()) {
					pair<int, int> current = neighborlabel.top();
					int x = current.first, y = current.second;
					dst.at<int>(x, y) = label;
					neighborlabel.pop();
					//将邻域入栈
					if (x - 1 >= 0 && y - 1 >= 0 && dst.at<int>(x - 1, y - 1) == 1)
					{
						neighborlabel.push(pair<int, int>(x - 1, y - 1));
					}
					if (x - 1 >= 0 && dst.at<int>(x - 1, y) == 1)
					{
						neighborlabel.push(pair<int, int>(x - 1, y));
					}
					if (x - 1 >= 0 && y + 1 <= cols && dst.at<int>(x - 1, y + 1) == 1)
					{
						neighborlabel.push(pair<int, int>(x - 1, y + 1));
					}
					if (y - 1 >= 0 && dst.at<int>(x, y - 1) == 1)
					{
						neighborlabel.push(pair<int, int>(x, y - 1));
					}
					if (y + 1 <= cols && dst.at<int>(x, y + 1) == 1)
					{
						neighborlabel.push(pair<int, int>(x, y + 1));
					}
					if (x + 1 <= rows && y - 1 >= 0 && dst.at<int>(x + 1, y - 1) == 1)
					{
						neighborlabel.push(pair<int, int>(x + 1, y - 1));
					}
					if (x + 1 <= rows && dst.at<int>(x + 1, y) == 1)
					{
						neighborlabel.push(pair<int, int>(x + 1, y));
					}
					if (x + 1 <= rows && y + 1 <= cols && dst.at<int>(x + 1, y + 1) == 1)
					{
						neighborlabel.push(pair<int, int>(x + 1, y + 1));
					}
				}
			}
		}
	}
}

//随机返回一个RGB三元组颜色值
Scalar randomcolor()
{
	uchar r = 255 * (rand() / (1.0 + RAND_MAX));
	uchar g = 255 * (rand() / (1.0 + RAND_MAX));
	uchar b = 255 * (rand() / (1.0 + RAND_MAX));
	return Scalar(b, g, r);
}

void addcolor(const Mat & src, Mat & dst)
{
	if (src.empty() ||
		src.type() != CV_32SC1)
	{
		return;
	}

	map<int, Scalar> colors;

	int rows = src.rows;
	int cols = src.cols;

	dst.release();
	dst.create(rows, cols, CV_8UC3);
	dst = Scalar::all(0);

	for (int i = 0; i < rows; i++)
	{
		const int* data_src = (int*)src.ptr<int>(i);
		uchar* data_dst = dst.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			int pixelValue = data_src[j];
			if (pixelValue > 1)
			{
				if (colors.count(pixelValue) <= 0) //如果之前还没有为这个label产生一个颜色，就调用RanColor函数随机对应一个颜色
				{
					colors[pixelValue] = randomcolor();
				}
				Scalar color = colors[pixelValue];
				*data_dst++ = color[0];       //因为当前dst设置为三通道，所以每个像素占3个位置
				*data_dst++ = color[1];
				*data_dst++ = color[2];
			}
			else  //背景
			{
				data_dst++;
				data_dst++;
				data_dst++;
			}
		}
	}
}

void addcolor1(const Mat & src, Mat & dst)
{
	if (src.empty() ||
		src.type() != CV_32SC1)
	{
		return;
	}

	int rows = src.rows;
	int cols = src.cols;

	dst.release();
	dst.create(rows, cols, CV_8UC3);
	dst = Scalar::all(0);

	for (int i = 0; i < rows; i++)
	{
		const int* data_src = (int*)src.ptr<int>(i);
		uchar* data_dst = dst.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			int pixelValue = data_src[j];
			if (pixelValue == label_max)
			{
				*data_dst++ = 255;
				*data_dst++ = 255;
				*data_dst++ = 255;
			}
			else  //背景
			{
				data_dst++;
				data_dst++;
				data_dst++;
			}
		}
	}
}

int main()
{
	Mat img = imread("E:/计算机视觉/exp5/horse_mask.png", 0);
	Mat img_binary;
	img.copyTo(img_binary);
	//阈值分割，灰度值大于127的置为1，否则置为0
	//在本次实验用到的图像中前景马是白色，在img_binary中变成了1，背景是黑色，在img_binary中变成了0
	threshold(img, img_binary, 127, 1, THRESH_BINARY);

	Mat img_label, img_label_seedfill;	//存储每个像素位置对应label的Mat结构
	Begin = clock();
	two_pass(img_binary, img_label);
	End = clock();
	duration[0] = (float)(End - Begin) * 1000 / CLK_TCK;
	Begin = clock();
	seedfill(img_binary, img_label_seedfill);
	End = clock();
	duration[1] = (float)(End - Begin) * 1000 / CLK_TCK;

	Mat img_result, img_result1, img_seedfill;
	addcolor(img_label, img_result);
	addcolor(img_label_seedfill, img_seedfill);
	addcolor1(img_label, img_result1);
	cout << "一共s有：" << maxlabel << " 个连通域" << endl;
	imshow("原图", img);
	imshow("连通域", img_result);
	imshow("只保留最大的一个连通域", img_result1);
	imshow("种子填充", img_seedfill);


	cout << "使用快速连通域算法耗时为：" << duration[0] << "ms" << endl;
	cout << "使用种子填充算法耗时为：" << duration[1] << "ms" << endl;
	waitKey(0);

	return 0;
}