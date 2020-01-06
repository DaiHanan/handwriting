//
// Created by lenovo on 2019/11/8.
//

#include "Neighbor.h"

int Neighbor::x[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
int Neighbor::y[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int Neighbor::direct[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

bool Neighbor::next(Point& point, const int& step, bool force) {
    if (!Point::check(point) && !force) return false;//该点是否合法

    switch (step) {
    case DIRECT_RIGHT://右
        point.y++;
        break;
    case DIRECT_RIGHT_TOP://右上
        point.y++;
        point.x--;
        break;
    case DIRECT_TOP://上
        point.x--;
        break;
    case DIRECT_LEFT_TOP://左上
        point.x--;
        point.y--;
        break;
    case DIRECT_LEFT://左
        point.y--;
        break;
    case DIRECT_LEFT_BOTTOM://左下
        point.x++;
        point.y--;
        break;
    case DIRECT_BOTTOM://下
        point.x++;
        break;
    case DIRECT_RIGHT_BOTTOM://右下
        point.x++;
        point.y++;
        break;
    default: return false;//动作不合法
    }
    //移动后不合法则回退
    if (force || Point::check(point)) {
        return true;
    }
    else {
        Neighbor::back(point, step);
        return false;
    }
}

bool Neighbor::back(Point& point, const int& step, bool force) {
    if (!Point::check(point)) return false;//该点是否合法

    switch (step) {//这里是反向！！
    case DIRECT_RIGHT://右
        point.y--;
        break;
    case DIRECT_RIGHT_TOP://右上
        point.x++;
        point.y--;
        break;
    case DIRECT_TOP://上
        point.x++;
        break;
    case DIRECT_LEFT_TOP://左上
        point.x++;
        point.y++;
        break;
    case DIRECT_LEFT://左
        point.y++;
        break;
    case DIRECT_LEFT_BOTTOM://左下
        point.y++;
        point.x--;
        break;
    case DIRECT_BOTTOM://下
        point.x--;
        break;
    case DIRECT_RIGHT_BOTTOM://右下
        point.x--;
        point.y--;
        break;
    default: return false;//动作不合法
    }
    //回退后不合法则前进
    if (Point::check(point)) {
        return true;
    }
    else {
        Neighbor::next(point, step);
        return false;
    }
}

Point Neighbor::nextOne(const Point& point, const int& step) {
    Point nextPoint = point;
    Neighbor::next(nextPoint, step, true);

    return nextPoint;
}

Point Neighbor::backOne(const Point& point, const int& step) {
    Point backPoint = point;
    Neighbor::back(backPoint, step, true);

    return backPoint;
}

int Neighbor::reverse(const int& direct) {
    switch (direct) {//调转方向
    case DIRECT_RIGHT://右
        return DIRECT_LEFT;
    case DIRECT_RIGHT_TOP://右上
        return DIRECT_LEFT_BOTTOM;
    case DIRECT_TOP://上
        return DIRECT_BOTTOM;
    case DIRECT_LEFT_TOP://左上
        return DIRECT_RIGHT_BOTTOM;
    case DIRECT_LEFT://左
        return DIRECT_RIGHT;
    case DIRECT_LEFT_BOTTOM://左下
        return DIRECT_RIGHT_TOP;
    case DIRECT_BOTTOM://下
        return DIRECT_TOP;
    case DIRECT_RIGHT_BOTTOM://右下
        return DIRECT_LEFT_TOP;
    default: return DIRECT_NULL;//动作不合法
    }
}

bool Neighbor::isLevel(const int& direct) {
    switch (direct) {//调转方向
    case DIRECT_RIGHT://右
    case DIRECT_TOP://上
    case DIRECT_LEFT://左
    case DIRECT_BOTTOM://下
        return true;
    case DIRECT_RIGHT_TOP://右上
    case DIRECT_LEFT_TOP://左上
    case DIRECT_LEFT_BOTTOM://左下
    case DIRECT_RIGHT_BOTTOM://右下
        return false;
    default: return false;//动作不合法
    }
}

bool Neighbor::isCorner(const int& direct) {
    if (!Neighbor::isValid(direct))
        return false;
    return !Neighbor::isLevel(direct);//合法时返回水平竖直相反值
}

bool Neighbor::isValid(const int& direct) {
    return direct >= 0 && direct < 8;
}

