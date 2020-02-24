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
    //�������
    explicit Calculate(vector<vector<int>>* _thinVal, vector<vector<int>>* _sourceVal);
    /**
     * ��ʼ����
     */
    void run();
    /**
     * ��ӡ�˵�����
     */
    void printPoints();
    /**
     * ��ӡ���ؾ���
     */
    void printMatrix();
    /*
    ��ӡԴͼ�����ؾ���
    */
    void printSourceMatrix();
    /**
     * ��ӡ·��
     */
    void printPaths();
    /*
    ������Ϣ
    */
    void save();

private:
    vector<vector<int>>& val;//���ؾ���
    vector<vector<int>>& source;//Դͼ�����ؾ���
    int width;//��
    int height;//��

    vector<Point> points;//�˵�
    vector<Path> paths;//·��

    /**
     * ϸ������
     */
    void toThin();
    /**
     * ��������Ƿ����
     * @param point ����
     * @return �Ƿ����
     */
    bool checkPoint(const Point& point);
   
    /**
     * ����˵�
     * @param points �˵㼯��
     */
    void getPoints(vector<Point>& points);

    /**
     * ����˵�
     * ��������Ϊ��д������ң��ϵ���
     */
    void sortPoints(vector<Point>& points);
    /**
     * �˵�����Ƚ���
     */
    static bool pointsComparator(const Point& point1, const Point& point2);

    /**
     * ����·��
     */
    void getPath();
    /**
     * ����·����������
     * �ݹ�������·����һ���ڵ�
     * @param pre ��ǰ�ڵ�����
     * @param move ��ͷ�ڵ㵽��ǰ�ڵ�Ķ�������
     * @param color ��ǰ·����ɫ
     */
    void searchNextPoint(const Point& now, vector<int>& move, int color);
    /**
     * �ݹ�-�����preDirect��������ĳ������ʱ�Ĵ���ʷ���
     * �ݹ��������������������ͬ�����������һ�����·��
     * �㷨�������������״γ���������ͬ�����һ���е�һ�����ֵķ���
     * @param now �����е�ĳһ��
     * @param preDirects ���쵽�õ�֮ǰ�ĵ�����ķ�������(�����now�ڵ�)(���޷�ǰ���ķ���)
     * @param directs ��������еķ�������
     * @param color �ݹ�ʱ��ѡ���ص���ɫ
     * @return �����췽��Ĵ���ʷ���
     */
    int findPathMainDirect(const Point& now, const vector<int>& preDirects, vector<int>& directs, const int& color = 1);
    /**
     * ɾ���ھ��д���ʷ�����Ե��ھӶ�
     * @param unRunNeighbors �ھ�
     */
    static void deleteOppositeDirectCouples(map<int, int>& unRunNeighbors);
    /**
     * �ж�ͼ���Ƿ��Ѿ����ߡ���
     * @return �Ƿ�����
     */
    bool isAllPassed();

    /**
     * ����ȫ�����ư뾶
     */
    void getRadii();
    /**
     * ������ư뾶
     */
    float getRadius(const Point& point);

    /*
    ����ͼƬ
    */
    void saveImage();
    /*
    �������ݿ�
    */
    void saveSql();
};

