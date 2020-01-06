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
    static string sourcePath;//���β�����ԭͼƬ·��
    bool isSuccess;//��Ϣ�Ƿ��óɹ�
    /**
     * �洢������Ϣ x, y
     * ֵ��1(��)/0(��)
     * _valָ��ʵ�ʵ�ַ
     */
    vector<vector<int>>* _val;
    //��ʼ��ͼ����Ϣ
    explicit BmpInfo(string path);
    ~BmpInfo();

    /**
     * ������ؾ�����
     * @return ���
     */
    int getWidth() { return this->bih.biWidth; };
    /**
     * ������ؾ��󳤶�
     * @return ����
     */
    int getHeight() { return this->bih.biHeight; };
    /**
     * ��ӡ������Ϣ
     * @return this
     */
    BmpInfo* printVal();
    /**
     * ��ӡbmp��Ϣ
     * @return this
     */
    BmpInfo* printInfo();
    /**
     * ���bmpʵ��
     * @param path
     * @return ��Ϣʵ��
     */
    static BmpInfo* getBmp(const string& path);

private:
    const string filePath;//�ļ�·��
    //ͼƬ������Ϣ
    myBITMAPINFOHEADER bih;
    BYTE* Buffer;
    long LineByteWidth;

    /**
     * ��ʼ�����ؾ���
     */
    void initVal();
    /**
     * ��ʼ��ͼ�λ�����Ϣ
     */
    void initInfo();
    /**
     * ���ĳ��λ�õ�����ֵ
     * @param X x����
     * @param Y y����
     * @param r red
     * @param g green
     * @param b blue
     * @return �Ƿ��óɹ�
     */
    bool getDIBColor(int X, int Y, BYTE* r, BYTE* g, BYTE* b);
};