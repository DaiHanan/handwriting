#pragma once

#include <climits>

typedef struct _lyl_point {
    int x;  //������
    int y;  //������

    _lyl_point(int xx = 0, int yy = 0) :x(xx), y(yy) {};
    /**
     * ���ĳ���Ƿ�Ϸ�
     * @param point ����
     * @return �Ƿ�Ϸ�
     */
    static bool check(const struct _lyl_point& point, int maxX = INT_MAX, int maxY = INT_MAX) {
        return point.x >= 0 && point.y >= 0 && point.x <= maxX && point.y <= maxY;
    };
} Point;