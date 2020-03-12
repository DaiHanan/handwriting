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
     * ��ĳ���ƽ�һ��
     * @param point ĳ����������
     * @param step ����
     * @param force �Ƿ�ǿ��ת��
     * @return �Ƿ�ɹ�
     */
    static bool next(Point& point, const int& step, bool force = false);
    /**
     * ��ĳ�����һ��
     * @param point ĳ����������
     * @param step Ҫ�����Ķ�����������㣩
     * @return �Ƿ�ɹ�
     */
    static bool back(Point& point, const int& step, bool force = false);
    /**
     * ���ĳ���ƽ�һ������һ������
     * @param point ĳ������
     * @param step ��һ��
     * @return ��һ������
     */
    static Point nextOne(const Point& point, const int& step);
    /**
     * ���ĳ�㵹��һ������һ������
     * @param point ĳ������
     * @param step Ҫ�����Ĳ���
     * @return ��һ������
     */
    static Point backOne(const Point& point, const int& step);
    /**
     * ���ص�ת���򣬼�������ھӵķ���
     * @param direct ��Ҫ��ת�ķ���
     * @return ��ת��ķ���
     */
    static int reverse(const int& direct);
    /**
     * �жϷ����Ƿ�ˮƽ����ֱ����
     * @param direct ĳ����
     * @return �Ƿ�ˮƽ��ֱ
     */
    static bool isLevel(const int& direct);
    /**
     * �жϷ����Ƿ�Ϊ�Խ��߷���
     * @param direct ĳ����
     * @return �Ƿ�Ϊ�Խ��߷���
     */
    static bool isCorner(const int& direct);
    /**
     * �жϷ����Ƿ�Ϸ�
     * @param direct
     * @return
     */
    static bool isValid(const int& direct);
    /*
    ���ظ÷�����������������ڷ��������
    @param direct λ���м�ķ���
    @return ���ڷ�������
    */
    static vector<int> getNeighborArr(const int& direct);
    /*
    ��÷����ַ��ַ���
    */
    static string getNeighborString(const int& direct);
};
