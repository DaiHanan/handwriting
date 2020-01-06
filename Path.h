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
     * ��ӡ����
     */
    void print() const;
    int getPointCount() const;
    /**
     * ���û��ư뾶
     * @param idx �õ��±�
     * @param radius �뾶��С
     * @return �Ƿ�Ϸ�
     */
    bool setRadius(int idx, float radius);
    /**
     * ��û��ư뾶
     * @param idx �õ��±�
     * @return �뾶��С -1�򲻺Ϸ� 0Ϊδ��ʼ��
     */
    float getRadius(int idx) const;
    /**
     * �����ʼ����
     * @return
     */
    const Point& getFromPoint() const;
    /**
     * ��ý�������
     * @return
     */
    const Point& getToPoint() const;
    /**
     * ���ĳһ������
     * @param idx move�����±�
     * @return �ò����� DIRECT_NULL�򲻺Ϸ�
     */
    int getStep(int idx) const;

private:
    Point from, to;//��ʼ���յ�
    vector<int> move;//·��
    vector<float> radius;//���ư뾶,���鳤�� = ��ĸ��� = ·������ + 1
    /**
     * ��ʼ����ʼ����
     * @param point ��ʼ����
     */
    void initFromPoint(const Point& point);
    /**
     * ������ʼ�����move�����ս�����
     */
    bool initToPoint();
    /**
     * ����һ���������ս�����
     * @param step ����
     * @return �Ƿ����ӳɹ�������Խ�磩
     */
    bool pushMove(int step);
    /**
     * ����һ���������ս�����
     * @return �Ƿ���˳ɹ�
     */
    bool popMove();
};

