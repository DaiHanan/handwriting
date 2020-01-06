//
// Created by lenovo on 2019/11/8.
//

#include "Neighbor.h"

int Neighbor::x[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
int Neighbor::y[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int Neighbor::direct[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

bool Neighbor::next(Point& point, const int& step, bool force) {
    if (!Point::check(point) && !force) return false;//�õ��Ƿ�Ϸ�

    switch (step) {
    case DIRECT_RIGHT://��
        point.y++;
        break;
    case DIRECT_RIGHT_TOP://����
        point.y++;
        point.x--;
        break;
    case DIRECT_TOP://��
        point.x--;
        break;
    case DIRECT_LEFT_TOP://����
        point.x--;
        point.y--;
        break;
    case DIRECT_LEFT://��
        point.y--;
        break;
    case DIRECT_LEFT_BOTTOM://����
        point.x++;
        point.y--;
        break;
    case DIRECT_BOTTOM://��
        point.x++;
        break;
    case DIRECT_RIGHT_BOTTOM://����
        point.x++;
        point.y++;
        break;
    default: return false;//�������Ϸ�
    }
    //�ƶ��󲻺Ϸ������
    if (force || Point::check(point)) {
        return true;
    }
    else {
        Neighbor::back(point, step);
        return false;
    }
}

bool Neighbor::back(Point& point, const int& step, bool force) {
    if (!Point::check(point)) return false;//�õ��Ƿ�Ϸ�

    switch (step) {//�����Ƿ��򣡣�
    case DIRECT_RIGHT://��
        point.y--;
        break;
    case DIRECT_RIGHT_TOP://����
        point.x++;
        point.y--;
        break;
    case DIRECT_TOP://��
        point.x++;
        break;
    case DIRECT_LEFT_TOP://����
        point.x++;
        point.y++;
        break;
    case DIRECT_LEFT://��
        point.y++;
        break;
    case DIRECT_LEFT_BOTTOM://����
        point.y++;
        point.x--;
        break;
    case DIRECT_BOTTOM://��
        point.x--;
        break;
    case DIRECT_RIGHT_BOTTOM://����
        point.x--;
        point.y--;
        break;
    default: return false;//�������Ϸ�
    }
    //���˺󲻺Ϸ���ǰ��
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
    switch (direct) {//��ת����
    case DIRECT_RIGHT://��
        return DIRECT_LEFT;
    case DIRECT_RIGHT_TOP://����
        return DIRECT_LEFT_BOTTOM;
    case DIRECT_TOP://��
        return DIRECT_BOTTOM;
    case DIRECT_LEFT_TOP://����
        return DIRECT_RIGHT_BOTTOM;
    case DIRECT_LEFT://��
        return DIRECT_RIGHT;
    case DIRECT_LEFT_BOTTOM://����
        return DIRECT_RIGHT_TOP;
    case DIRECT_BOTTOM://��
        return DIRECT_TOP;
    case DIRECT_RIGHT_BOTTOM://����
        return DIRECT_LEFT_TOP;
    default: return DIRECT_NULL;//�������Ϸ�
    }
}

bool Neighbor::isLevel(const int& direct) {
    switch (direct) {//��ת����
    case DIRECT_RIGHT://��
    case DIRECT_TOP://��
    case DIRECT_LEFT://��
    case DIRECT_BOTTOM://��
        return true;
    case DIRECT_RIGHT_TOP://����
    case DIRECT_LEFT_TOP://����
    case DIRECT_LEFT_BOTTOM://����
    case DIRECT_RIGHT_BOTTOM://����
        return false;
    default: return false;//�������Ϸ�
    }
}

bool Neighbor::isCorner(const int& direct) {
    if (!Neighbor::isValid(direct))
        return false;
    return !Neighbor::isLevel(direct);//�Ϸ�ʱ����ˮƽ��ֱ�෴ֵ
}

bool Neighbor::isValid(const int& direct) {
    return direct >= 0 && direct < 8;
}

