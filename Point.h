#pragma once

#include <climits>

typedef struct _lyl_point {
    int x;  //纵坐标
    int y;  //横坐标

    _lyl_point(int xx = 0, int yy = 0) :x(xx), y(yy) {};
    /**
     * 检查某点是否合法
     * @param point 坐标
     * @return 是否合法
     */
    static bool check(const struct _lyl_point& point, int maxX = INT_MAX, int maxY = INT_MAX) {
        return point.x >= 0 && point.y >= 0 && point.x <= maxX && point.y <= maxY;
    };
} Point;