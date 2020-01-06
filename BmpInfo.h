#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <malloc.h>
#include <vector>
#include "BmpStruct.h"
#include "Log.h"
#include <utility>
#include <iostream>

using namespace std;

class BmpInfo {
public:
    static string sourcePath;//本次操作的原图片路径
    bool isSuccess;//信息是否获得成功
    /**
     * 存储像素信息 x, y
     * 值：1(黑)/0(白)
     * _val指向实际地址
     */
    vector<vector<int>>* _val;
    //初始化图形信息
    explicit BmpInfo(string path);
    ~BmpInfo();

    /**
     * 获得像素矩阵宽度
     * @return 宽度
     */
    int getWidth() { return this->bih.biWidth; };
    /**
     * 获得像素矩阵长度
     * @return 长度
     */
    int getHeight() { return this->bih.biHeight; };
    /**
     * 打印矩阵信息
     * @return this
     */
    BmpInfo* printVal();
    /**
     * 打印bmp信息
     * @return this
     */
    BmpInfo* printInfo();
    /**
     * 获得bmp实例
     * @param path
     * @return 信息实例
     */
    static BmpInfo* getBmp(const string& path);

private:
    const string filePath;//文件路径
    //图片基本信息
    myBITMAPINFOHEADER bih;
    BYTE* Buffer;
    long LineByteWidth;

    /**
     * 初始化像素矩阵
     */
    void initVal();
    /**
     * 初始化图形基础信息
     */
    void initInfo();
    /**
     * 获得某个位置的像素值
     * @param X x坐标
     * @param Y y坐标
     * @param r red
     * @param g green
     * @param b blue
     * @return 是否获得成功
     */
    bool getDIBColor(int X, int Y, BYTE* r, BYTE* g, BYTE* b);
};