#pragma once

#define DIRECT_NULL (-1)
#define DIRECT_RIGHT 0
#define DIRECT_RIGHT_TOP 1
#define DIRECT_TOP 2
#define DIRECT_LEFT_TOP 3
#define DIRECT_LEFT 4
#define DIRECT_LEFT_BOTTOM 5
#define DIRECT_BOTTOM 6
#define DIRECT_RIGHT_BOTTOM 7

#include "Point.h"
#include <vector>
#include <string>
using namespace std;

class Neighbor {
public:
    static int x[8], y[8], direct[8];
    /**
     * 将某点推进一步
     * @param point 某点坐标引用
     * @param step 动作
     * @param force 是否强制转换
     * @return 是否成功
     */
    static bool next(Point& point, const int& step, bool force = false);
    /**
     * 将某点回退一步
     * @param point 某点坐标引用
     * @param step 要撤销的动作（反向计算）
     * @return 是否成功
     */
    static bool back(Point& point, const int& step, bool force = false);
    /**
     * 获得某点推进一步的下一个坐标
     * @param point 某点坐标
     * @param step 下一步
     * @return 下一步坐标
     */
    static Point nextOne(const Point& point, const int& step);
    /**
     * 获得某点倒退一步的上一个坐标
     * @param point 某点坐标
     * @param step 要撤销的步骤
     * @return 上一步坐标
     */
    static Point backOne(const Point& point, const int& step);
    /**
     * 返回调转方向，即相对于邻居的方向
     * @param direct 需要调转的方向
     * @return 调转后的方向
     */
    static int reverse(const int& direct);
    /**
     * 判断方向是否水平或竖直方向
     * @param direct 某方向
     * @return 是否水平竖直
     */
    static bool isLevel(const int& direct);
    /**
     * 判断方向是否为对角线方向
     * @param direct 某方向
     * @return 是否为对角线方向
     */
    static bool isCorner(const int& direct);
    /**
     * 判断方向是否合法
     * @param direct
     * @return
     */
    static bool isValid(const int& direct);
    /*
    返回该方向包括相邻两个相邻方向的数组
    @param direct 位于中间的方向
    @return 相邻方向数组
    */
    static vector<int> getNeighborArr(const int& direct);
    /*
    获得方向字符字符串
    */
    static string getNeighborString(const int& direct);
};
