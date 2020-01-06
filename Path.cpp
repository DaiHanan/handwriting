//
// Created by lenovo on 2019/11/8.
//

#include "Path.h"
#include "Log.h"

Path::Path(const Point& from) {
    this->initFromPoint(from);
    this->initToPoint();
    this->radius = vector<float>(1);
}

Path::Path(const Point& from, const vector<int>& move) {
    this->initFromPoint(from);
    this->move = move;
    this->radius = vector<float>(this->move.size() + 1);
    this->initToPoint();
}

bool Path::initToPoint() {
    this->to = this->from;
    for (int& step : this->move) {//һ��һ������
        this->isValid = Neighbor::next(this->to, step);
    }
    return this->isValid;
}

void Path::initFromPoint(const Point& point) {
    this->isValid = Point::check(point);
    this->from = point;
}

bool Path::pushMove(int step) {
    if (Neighbor::next(this->to, step)) {//����ƶ��ɹ�
        this->move.push_back(step);//���Ӳ���
        return true;
    }
    return false;
}

bool Path::popMove() {
    if (this->move.size() > 0 && Neighbor::back(this->to, this->move.back())) {//������˳ɹ�
        this->move.pop_back();//���ٲ���
        return true;
    }
    return false;
}

void Path::print() const {
    ostringstream& ss = Log::ss;
    ss << "[" << this->from.x << "," << this->from.y << "] ��" << "[" << this->to.x << "," << this->to.y << "]" << endl;
    ss << "��������\n(";
    if (!this->move.empty()) {
        ss << this->move[0];
        for (int i = 1; i < this->move.size(); i++) {
            ss << "," << this->move[i];
        }
    }
    ss << ")" << endl;
    ss << "�뾶����\n(";
    if (!this->radius.empty()) {
        ss << this->radius[0];
        for (int i = 1; i < this->radius.size(); i++) {
            ss << "," << this->radius[i];
        }
    }
    ss << ")" << endl;
    Log::write();
}

int Path::getPointCount() const {
    return (int)this->move.size() + 1;
}

bool Path::setRadius(int idx, float radius) {
    if (idx > this->move.size() + 1 || idx < 0) return false;//Խ��
    this->radius[idx] = radius;
    return true;
}

float Path::getRadius(int idx) const {
    if (idx > this->move.size() + 1 || idx < 0) return false;//Խ��
    return this->radius[idx];
}

const Point& Path::getFromPoint() const {
    return this->from;
}

const Point& Path::getToPoint() const {
    return this->to;
}

int Path::getStep(int idx) const {
    if (idx > this->move.size() || idx < 0) return DIRECT_NULL;
    return this->move[idx];
}
