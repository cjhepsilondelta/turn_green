// jpg_turn_green.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <io.h>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;
using namespace cv;
int r_green=199, g_green=237, b_green=204;
double dis_global = 60;
void turn_green(Mat& img)
{
	long num_color = 0;
	double distance=0.0;
	for (int i = 0; i != img.rows; i++)
	{
		for (int j = 0; j != img.cols; j++)
		{
			Vec3b& color = img.at<Vec3b>(i, j);
			distance = sqrt(pow((color[0] - 255), 2) + pow((color[1] - 255), 2) + pow((color[2] - 255), 2));
			++num_color;
			if (distance < dis_global)
			{
				//color = (r_green, g_green, b_green);
				color[0] = b_green;
				color[1] = g_green;
				color[2] = r_green;
				//img.at<Vec3b>(i, j)[0] = b_green;
				//img.at<Vec3b>(i, j)[1] = g_green;
				//img.at<Vec3b>(i, j)[2] = r_green;
			}
		}
	}
	//cout << "num_color" <<num_color << endl;
	//cout << "rows:" << img.rows << "  cols:" << img.cols << endl;
}
void color_reduce(Mat& img, int div)
{
	for (int i = 0; i != img.rows; i++)
	{
		for (int j=0;j!=img.cols;j++)
		{
			img.at<Vec3b>(i, j)[0] = img.at<Vec3b>(i, j)[0] / div * div + div / 2;
			img.at<Vec3b>(i, j)[1] = img.at<Vec3b>(i, j)[1] / div * div + div / 2;
			img.at<Vec3b>(i, j)[2] = img.at<Vec3b>(i, j)[2] / div * div + div / 2;
			//Vec3b a = img.at<Vec3b>(i, j);
			//cout << "123" << endl;
		}
	}
}
void getFiles(string path, vector<string>& files, bool recursive, string find_type)
{
	//文件句柄  
	long   hFile = 0;
	string file_name;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0 && recursive)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files, recursive, find_type);
			}
			else
			{
				file_name = fileinfo.name;
				if (file_name.find(find_type) != string::npos)
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
void color_practice(Mat& img)
{
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			int mu = img.at<Vec3b>(i, j)[0] + img.at<Vec3b>(i, j)[1] + img.at<Vec3b>(i, j)[2];
			mu /= 3;
			int stdev = sqrt(double((img.at<Vec3b>(i, j)[0] - mu)*(img.at<Vec3b>(i, j)[0] - mu) + (img.at<Vec3b>(i, j)[1] - mu)*(img.at<Vec3b>(i, j)[1] - mu) + (img.at<Vec3b>(i, j)[2] - mu)*(img.at<Vec3b>(i, j)[2] - mu)) / 3);
			for (int k = 0; k < 3; k++)
				img.at<Vec3b>(i, j)[k] = stdev;
		}
	}
}
void string_split(vector<string>& str_vec,string str_org, string split_str)
{
	if (str_org.find(split_str) != string::npos)
	{
		
	}
	else
		str_vec.push_back(str_org);
}
vector<string> splitWithStl(const string &str, const string &pattern)
{
	vector<string> resVec;

	if ("" == str)
	{
		return resVec;
	}
	//方便截取最后一段数据
	string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != string::npos)
	{
		string x = strs.substr(0, pos);
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return resVec;
}
int main()
{
	string input_path = "D:\\Downloads\\Compressed\\89268708OpenCV3编程入门+代码\\img";
	string output_path = "D:\\Downloads\\Compressed\\89268708OpenCV3编程入门+代码\\out";
	vector<string> files;
	getFiles(input_path, files, false, ".jpg");
	Mat img;
	string img_name;
	vector<string> img_name_vec;
	DWORD start_time,end_time,all_time;
	all_time = GetTickCount();
	for (int i=0,fsize=files.size();i<fsize;i++)
	{
		start_time = GetTickCount();
		img = imread(files[i]);
		turn_green(img);
		img_name_vec = splitWithStl(files[i],"\\");
		img_name = img_name_vec[img_name_vec.size() - 1];
		imwrite(output_path + "\\" + img_name, img);
		end_time = GetTickCount();
		cout << img_name << "转换完毕" <<endl;
		cout << "存储路径：" << output_path + "\\" + img_name << endl;
		cout << "单图运行时间:" << (end_time - start_time)/1000.0 << "s 总运行时间:" << (end_time - all_time)/1000.0 << "s" << endl;
	}
}