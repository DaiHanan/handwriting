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
    for (int& step : this->move) {//一步一步更新
        this->isValid = Neighbor::next(this->to, step);
    }
    return this->isValid;
}

void Path::initFromPoint(const Point& point) {
    this->isValid = Point::check(point);
    this->from = point;
}

bool Path::pushMove(int step) {
    if (Neighbor::next(this->to, step)) {//如果移动成功
        this->move.push_back(step);//增加步骤
        return true;
    }
    return false;
}

bool Path::popMove() {
    if (this->move.size() > 0 && Neighbor::back(this->to, this->move.back())) {//如果回退成功
        this->move.pop_back();//减少步骤
        return true;
    }
    return false;
}

void Path::print() const {
    ostringstream& ss = Log::ss;
    ss << "[" << this->from.x << "," << this->from.y << "] →" << "[" << this->to.x << "," << this->to.y << "]" << endl;
    ss << "方向数组\n(";
    if (!this->move.empty()) {
        ss << this->move[0];
        for (int i = 1; i < this->move.size(); i++) {
            ss << "," << this->move[i];
        }
    }
    ss << ")" << endl;
    ss << "半径数组\n(";
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
    if (idx > this->move.size() + 1 || idx < 0) return false;//越界
    this->radius[idx] = radius;
    return true;
}

float Path::getRadius(int idx) const {
    if (idx > this->move.size() + 1 || idx < 0) return false;//越界
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
