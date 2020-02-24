#pragma once

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <iostream>
#include "Neighbor.h"
#include "Log.h"
#include "Path.h"
#include "ImageFactory.h"

using namespace std;

class Calculate {
public:
    //载入矩阵
    explicit Calculate(vector<vector<int>>* _thinVal, vector<vector<int>>* _sourceVal);
    /**
     * 开始计算
     */
    void run();
    /**
     * 打印端点坐标
     */
    void printPoints();
    /**
     * 打印像素矩阵
     */
    void printMatrix();
    /*
    打印源图像像素矩阵
    */
    void printSourceMatrix();
    /**
     * 打印路径
     */
    void printPaths();
    /*
    保存信息
    */
    void save();

private:
    vector<vector<int>>& val;//像素矩阵
    vector<vector<int>>& source;//源图像像素矩阵
    int width;//宽
    int height;//高

    vector<Point> points;//端点
    vector<Path> paths;//路径

    /**
     * 细化矩阵
     */
    void toThin();
    /**
     * 检查坐标是否出界
     * @param point 坐标
     * @return 是否出界
     */
    bool checkPoint(const Point& point);
   
    /**
     * 计算端点
     * @param points 端点集合
     */
    void getPoints(vector<Point>& points);

    /**
     * 排序端点
     * 简单排序，因为书写大多左到右，上到下
     */
    void sortPoints(vector<Point>& points);
    /**
     * 端点排序比较器
     */
    static bool pointsComparator(const Point& point1, const Point& point2);

    /**
     * 计算路径
     */
    void getPath();
    /**
     * 计算路径辅助函数
     * 递归搜索该路径下一个节点
     * @param pre 当前节点坐标
     * @param move 从头节点到当前节点的动作数组
     * @param color 当前路径颜色
     */
    void searchNextPoint(const Point& now, vector<int>& move, int color);
    /**
     * 递归-计算从preDirect方向来的某点延伸时的大概率方向
     * 递归结束条件：出现两次相同方向或遇到下一个多叉路口
     * 算法：方向数组中首次出现两次相同方向或一次中第一个出现的方向
     * @param now 延伸中的某一点
     * @param preDirects 延伸到该点之前的点过来的方向数组(相对于now节点)(即无法前进的方向)
     * @param directs 延伸过程中的方向数组
     * @param color 递归时可选像素的颜色
     * @return 该延伸方向的大概率方向
     */
    int findPathMainDirect(const Point& now, const vector<int>& preDirects, vector<int>& directs, const int& color = 1);
    /**
     * 删除邻居中大概率方向相对的邻居对
     * @param unRunNeighbors 邻居
     */
    static void deleteOppositeDirectCouples(map<int, int>& unRunNeighbors);
    /**
     * 判断图像是否已经‘走’完
     * @return 是否走完
     */
    bool isAllPassed();

    /**
     * 计算全部绘制半径
     */
    void getRadii();
    /**
     * 计算绘制半径
     */
    float getRadius(const Point& point);

    /*
    保存图片
    */
    void saveImage();
    /*
    保存数据库
    */
    void saveSql();
};

