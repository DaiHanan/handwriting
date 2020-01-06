#include "ImageFactory.h"

bool ImageFactory::output(const vector<vector<int>>& val, string path)
{
	if (val.empty() || val[0].empty())//像素矩阵非法
		return false;
	vector<vector<int>> tmp = vector<vector<int>>(val.size(), vector<int>(val[0].size(), 0));
	return ImageFactory::output(val, tmp, path);
}

bool ImageFactory::output(const vector<vector<int>>& val, const vector<vector<int>>& source, string path)
{
	if (val.empty() || val[0].empty())//像素矩阵非法
		return false;
	//创建三通道图
	cv::Mat image(val.size(), val[0].size(), CV_8UC3);
	//设置像素值
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			if (val[i][j] != 0) {//如果有笔画
				image.at<cv::Vec3b>(i, j) =
					cv::Vec3b(val[i][j] * 60 % 255,
						val[i][j] * 80 % 255,
						val[i][j] * 100 % 255);
			}
			else if (source[i][j] != 0) {
				image.at<cv::Vec3b>(i, j) =
					cv::Vec3b(235, 235, 235);
			}
			else {//背景
				image.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
			}
		}
	}
	//保存
	//cv::imshow("原图", image);
	if (path.empty())
		path = BmpInfo::sourcePath.substr(0, BmpInfo::sourcePath.size() - 4) + "_r.bmp";
	cv::imwrite(path, image);
	return true;
}
