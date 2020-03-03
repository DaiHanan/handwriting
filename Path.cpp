//
// Created by lenovo on 2019/11/8.
//

#include "Path.h"
#include "Log.h"
#include <algorithm>

Path::Path(const Point& from) {
    this->initFromPoint(from);
    this->initToPoint();
    this->radius = vector<float>(1);
}

Path::Path(const Point& from, const vector<int>& move) {
    this->initFromPoint(from);
    this->initToPoint();
    this->radius = vector<float>(1);
    for (const int& step : move) {
        if (!this->pushMove(step)) {
            throw "初始化路径失败";
        }
    }
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
    vector<int>& move = this->move;

    if (Neighbor::next(this->to, step)) {//如果移动成功
        move.push_back(step);//增加步骤
        int size = move.size();
        this->radius.emplace_back(0);//扩大半径数组

        //更新连续方向数组，注意会重复出现
        if (//出现4次以上
            size > 3 && 
            find_if_not(
                move.rbegin(), move.rend(), 
                [step](int item) {return item == step; }) - move.rbegin() >= 4
            ) {
            this->moveMore.emplace_back(step);
        }
        if (//出现三次
            size >= 3 &&
            find_if_not(
                move.rbegin(), move.rend(),
                [step](int item) {return item == step; }) - move.rbegin() == 3
            ) {
            this->move3.emplace_back(step);
        }
        if (//出现两次
            size >= 2 &&
            find_if_not(
                move.rbegin(), move.rend(),
                [step](int item) {return item == step; }) - move.rbegin() == 2
            ) {
            this->move2.emplace_back(step);
        }
        

        return true;
    }
    return false;
}

bool Path::popMove() {
    if (this->move.size() > 0 && Neighbor::back(this->to, this->move.back())) {//如果回退成功
        this->move.pop_back();//减少步骤
        this->radius.pop_back();//减少半径
        return true;
    }
    return false;
}

const vector<int>& Path::getMove(int num) const 
{
    switch (num)
    {
    case 1:
        return this->move;
    case 2:
        return this->move2;
    case 3:
        return this->move3;
    case 4:
        return this->moveMore;
    default:
        throw "move编号非法";
    }
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
    if (idx == -1) idx = this->move.size() - 1;//最后一个
    if (idx > this->move.size() || idx < -1) return DIRECT_NULL;
    return this->move[idx];
}
