#pragma once
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "BmpInfo.h"

using namespace std;

class ImageFactory
{
public:
	static bool output(const vector<vector<int>>& val, string path = "");

	static bool output(const vector<vector<int>>& val, const vector<vector<int>>& source, string path = "");
};

