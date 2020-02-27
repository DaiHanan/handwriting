#include "BmpInfo.h"
#include "Calculate.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    //获得路径      
    string thinPath = "D:/01_files/05_upward/02_school/02_handwriting/02_pics/page01/words/004(255,370).bmp", sourcePath;
    if (argc > 1)
        thinPath = argv[1];
    sourcePath = thinPath.substr(0, thinPath.size() - 4) + "_s.bmp";

    //获得BmpInfo实例
    BmpInfo* thinBmp = BmpInfo::getBmp(thinPath);
    BmpInfo*sourceBmp = BmpInfo::getBmp(sourcePath);
    /*cout << thinBmp->getWidth() << " " << sourceBmp->getWidth();
    if (!thinBmp || !sourceBmp ||
        thinBmp->getWidth() != sourceBmp->getWidth() ||
        thinBmp->getHeight() != sourceBmp->getHeight()
        ) return 1;*/
    //输出初始信息
    //thinBmp->printInfo()->printVal();

    //获得Calculate实例
    Calculate* calculate = new Calculate(thinBmp->_val, sourceBmp->_val);
    //开始计算
    calculate->run();

    //Log::write("----------------------------------------------------\n\n");
    //获得端点
    //calculate->printPoints();
    //获得操作时矩阵
    //calculate->printMatrix();
    //获得路径
    calculate->printPaths();

    //保存
    calculate->save();

    return 0;
}
