#pragma once

#include "Point.h"
#include "Neighbor.h"
#include <vector>

using namespace std;

class Path {
public:
    Path(const Point& from);
    Path(const Point& from, const vector<int>& move);

    bool isValid;
    /**
     * 打印功能
     */
    void print() const;
    int getPointCount() const;
    /**
     * 设置绘制半径
     * @param idx 该点下标
     * @param radius 半径大小
     * @return 是否合法
     */
    bool setRadius(int idx, float radius);
    /**
     * 获得绘制半径
     * @param idx 该点下标
     * @return 半径大小 -1则不合法 0为未初始化
     */
    float getRadius(int idx) const;
    /**
     * 获得起始坐标
     * @return
     */
    const Point& getFromPoint() const;
    /**
     * 获得结束坐标
     * @return
     */
    const Point& getToPoint() const;
    /**
     * 获得某一步方向
     * @param idx move数组下标, -1为数组最后一个元素
     * @return 该步方向 DIRECT_NULL则不合法
     */
    int getStep(int idx) const;
    /**
     * 增加一步，更新终结坐标和半径数组大小
     * @param step 动作
     * @return 是否增加成功（有无越界）
     */
    bool pushMove(int step);
    /**
     * 回退一步，更新终结坐标
     * @return 是否回退成功
     */
    bool popMove();
    /**
     * 获得move数组，无法更新
     * @return move数组
     */
    const vector<int>& getMove(int num = 1) const;

private:
    Point from, to;//起始和终点
    vector<int> move;//路径
    vector<float> radius;//绘制半径,数组长度 = 点的个数 = 路径数组 + 1

    vector<int> move2;//存储所有连续两次的方向
    vector<int> move3;//存储所有连续三次的方向
    vector<int> moveMore;//存储所有连续超过三次的方向

    /**
     * 初始化起始坐标
     * @param point 起始坐标
     */
    void initFromPoint(const Point& point);
    /**
     * 根据起始坐标和move计算终结坐标
     */
    bool initToPoint();
};

