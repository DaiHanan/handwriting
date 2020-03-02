//
// Created by lenovo on 2019/11/5.
//

#include "Calculate.h"
#include "MysqlFactory.h"

Calculate::Calculate(vector<vector<int>>* _thinVal, vector<vector<int>>* _sourceVal) : val(*_thinVal), source(*_sourceVal) {
    this->height = this->val.size();
    this->width = this->height > 0 ? this->val[0].size() : 0;
}

void Calculate::run() {
    this->toThin();
    this->getPoints(this->points);
    this->sortPoints(this->points);
    this->getPath();
    this->getRadii();
}

void Calculate::save()
{
    this->saveImage();
    //this->saveSql();//该行将字体信息存入数据库，取消后请勿重复运行,to余荔恒
}

void Calculate::toThin() {
    vector<vector<int>>& vals = this->val;
    int rows[] = { 0, -1, 0, 1 };//i 右上左下
    int cols[] = { 1, 0, -1, 0 };//j
    for (int i = 1; i < vals.size() - 1; i++) {
        for (int j = 1; j < vals[0].size() - 1; j++) {
            if (vals[i][j] == 1) {//黑色
                int count = 0;
                for (int k = 0; k < 4; k++) {//四个方向
                    if (vals[i + rows[k]][j + cols[k]] == 1)
                        count++;
                }
                if (count == 2) {//梯形
                    //四种情况
                    if (
                        (vals[i + rows[0]][j + cols[0]] == 1 && vals[i + rows[1]][j + cols[1]] == 1) ||//右上
                        (vals[i + rows[1]][j + cols[1]] == 1 && vals[i + rows[2]][j + cols[2]] == 1) ||//上左
                        (vals[i + rows[2]][j + cols[2]] == 1 && vals[i + rows[3]][j + cols[3]] == 1) ||//左下
                        (vals[i + rows[3]][j + cols[3]] == 1 && vals[i + rows[0]][j + cols[0]] == 1) //右下
                        )
                        vals[i][j] = 0;
                }
                else if (count == 3) {//凸形
                    vals[i][j] = 0;
                }
            }
        }
    }
}

bool Calculate::checkPoint(const Point& point) {
    return Point::check(point, this->height - 1, this->width - 1);
}

void Calculate::getPoints(vector<Point>& _points) {
    //八近邻下标
    int(&xx)[8] = Neighbor::x;
    int(&yy)[8] = Neighbor::y;
    //寻找端点
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            //对每个黑色像素点遍历
            if (this->val[i][j] == 1) {
                int neighbourCount = 0;//邻居数量
                for (int k = 0; k < 8; k++) {
                    //八近邻的纵坐标和横坐标
                    int x = i + xx[k], y = j + yy[k];
                    if (this->checkPoint(Point(x, y))
                        && this->val[x][y] == 1
                        && ++neighbourCount >= 2)
                        break;//超过2个不再继续
                }
                if (neighbourCount == 1)//只有一个邻居则为端点
                    _points.emplace_back(Point(i, j));
            }
        }
    }
    //如果没有端点时，则存在圆型循环，遍历找到一个未遍历点返回
    if (_points.empty()) {
        for (int j = 0; j < this->width; j++) {
            for (int i = 0; i < this->height; i++) {
                //对每个黑色像素点遍历
                if (this->val[i][j] == 1) {//未遍历点
                    _points.emplace_back(Point(i, j));
                    return;
                }
            }
        }
    }

}

void Calculate::printPoints() {
    ostringstream& ss = Log::ss;
    ss << "端点集合为：" << endl;
    for (Point& point : this->points) {
        ss << "[" << point.x << "," << point.y << "]";
    }
    ss << endl;
    Log::write();
}

void Calculate::sortPoints(vector<Point>& points) {
    sort(points.begin(), points.end(), Calculate::pointsComparator);
}
bool Calculate::pointsComparator(const Point& point1, const Point& point2) {
    //    if(abs(point1.y - point2.y) < 10) {//横向相似时比较纵向
    //        return point1.x < point2.x;
    //    } else return point1.y < point2.y;//比较横向
    return point1.x + point1.y < point2.x + point2.y;
}

void Calculate::getPath() {
    int color = 1;//路径颜色
    //取出所有端点，每取得一个路径，删掉已遍历端点重新搜索并排序端点
    vector<Point> points = this->points;
    while (!points.empty()) {
        for (int i = 0; i < points.size(); i++) {
            const Point& point = points[i];
            if (this->val[point.x][point.y] == 1) {//如果之前的路径没有经过该端点
                this->val[point.x][point.y] = ++color;//更新节点颜色
                Path path(point);
                this->searchNextPoint(point, path, color);//递归找路径
                this->paths.emplace_back(path);//找到后加入路径数组
                //取得一个路径，重新搜索并排序端点
                points.erase(points.begin() + i);
                this->getPoints(points);
                this->sortPoints(points);
                break;
            }
            else {//不能通过时直接删除
                points.erase(points.begin() + i--);
            }
        }
    }
    for (const Point& point : points) {
        if (this->val[point.x][point.y] == 1) {//如果之前的路径没有经过该端点
            this->val[point.x][point.y] = ++color;//更新节点颜色
            Path path(point);
            this->searchNextPoint(point, path, color);//递归找路径

            this->paths.emplace_back(path);//加入路径数组
        }
    }
    /**
     * 若图像仍有未遍历点，进入循环
     */
    while (!this->isAllPassed()) {
        //this->printMatrix();
        //寻找子端点
        vector<Point> subPoints;
        this->getPoints(subPoints);
        //遍历所有子端点
        for (const Point& point : subPoints) {
            if (this->val[point.x][point.y] == 1) {//如果之前的路径没有经过该端点
                this->val[point.x][point.y] = ++color;//更新节点颜色
                Path path(point);
                this->searchNextPoint(point, path, color);//递归找路径

                this->paths.emplace_back(path);//加入路径数组
            }
        }
    }
}
void Calculate::searchNextPoint(const Point& now, Path& path, int color) {
    //找到所有没有走过的邻居
    //map: first 该邻居所属方向 second 该延伸的大概率方向
    map<int, int> unRunNeighbors;
    for (int i = 0; i < 8; i++) {
        Point nextPoint = Neighbor::nextOne(now, i);
        int x = nextPoint.x, y = nextPoint.y;
        if (this->checkPoint(nextPoint) && this->val[x][y] == 1) {
            unRunNeighbors[i] = DIRECT_NULL;//存储方向
        }
    }
    //递归判断
    if (unRunNeighbors.empty()) return;//没有可前进的邻居
    //下一步
    int nextStep = DIRECT_NULL;
    if (unRunNeighbors.size() == 1) {//单个分支
        nextStep = unRunNeighbors.begin()->first;
        int backCount = this->isPathFinished(path, nextStep);//后退步数
        if (backCount > -1) {//提前结束路径
            Point backPoint = now;
            for (int i = 0; i < backCount; i++) {
                //重置像素color
                this->val[backPoint.x][backPoint.y] = 1;
                //后退
                Neighbor::back(backPoint, path.getStep(-1));
                path.popMove();
            }
            return;
        }
    }
    else {//多个分支
        vector<int> lengths;//每个分支参与计算大概率方向的长度
        //寻找每个分支的大概率方向
        for (auto& unRunNeighbor : unRunNeighbors) {
            /**
             * 因为从八近邻出发，所以不需要让其出发后再经过其他近邻
             * 需要根据相邻方向计算前置方向
             * 如果是水平或竖直，需要添加该方向以及左右各2个方向
             * 如果是对角方向，需要添加该方向以及左右各1个方向
             */
            vector<int> preDirects = { Neighbor::reverse(unRunNeighbor.first) };
            preDirects.emplace_back(Neighbor::reverse((unRunNeighbor.first + 1) % 8));
            preDirects.emplace_back(Neighbor::reverse((unRunNeighbor.first - 1 + 8) % 8));
            if (Neighbor::isLevel(unRunNeighbor.first)) {//水平额外左右2个
                preDirects.emplace_back(Neighbor::reverse((unRunNeighbor.first + 2) % 8));
                preDirects.emplace_back(Neighbor::reverse((unRunNeighbor.first - 2 + 8) % 8));
            }
            vector<int> directs(1, unRunNeighbor.first);//已递归方向数组
            unRunNeighbor.second = this->findPathMainDirect(//计算大概率方向
                Neighbor::nextOne(now, unRunNeighbor.first), preDirects, directs
            );
            lengths.emplace_back(directs.size());//更新长度
        }
        /**
         * 过滤掉满足条件的路径
         * 条件：自身长度等于1，相邻方向存在路径且长度均大于1
         */
        if (lengths.size() > 2) {
            int targetIdx = 0;
            //找到必要条件路径的下标
            while (
                lengths.size() > 2 &&//存在相邻方向
                (targetIdx = find(lengths.begin() + targetIdx, lengths.end(), 1) - lengths.begin()) < lengths.size() &&//该路径长度为1
                lengths[(targetIdx + 1) % lengths.size()] > 1 &&//右
                lengths[(targetIdx - 1 + lengths.size()) % lengths.size()] > 1//左
                ) {
                //再判断是否为路径方向相邻
                auto targetIter = unRunNeighbors.begin();//目标
                for (int i = 0; i < targetIdx; i++) {
                    ++targetIter;
                }
                int targetDirect = targetIter->first;//目标方向
                //判断相邻方向是否存在
                if (
                    unRunNeighbors.find((targetDirect + 1) % 8) != unRunNeighbors.end() && //右
                    unRunNeighbors.find((targetDirect - 1 + 8) % 8) != unRunNeighbors.end() //左
                    ) {
                    //存在→删除该方向
                    unRunNeighbors.erase(targetIter);
                    lengths.erase(lengths.begin() + targetIdx);
                }
            }
        }
        /**
         * 当move数组不为空时，为端点
         * 一定存在已经经过的路径的大概率方向
         * 当move数组为空时，为圆形循环点
         * 根据规律，圆形出发点大概率方向向下
         */
        int preDirect = DIRECT_NULL;
        const vector<int>& move = path.getMove();
        if (!move.empty()) {
            //寻找已经经过的路径的大概率方向
            vector<int> directs(1, Neighbor::reverse(move.back()));//已递归方向数组
            preDirect = findPathMainDirect(Neighbor::backOne(now, move.back()), { move.back() }, directs, color);
            preDirect = Neighbor::reverse(preDirect);//翻转方向，因为是反向推断方向
        }
        else {
            preDirect = DIRECT_BOTTOM;
        }

        //寻找前进方向
        //寻找第一个与已经过路径大概率方向相同的方向进行前进
        for (auto& unRunNeighbor : unRunNeighbors) {
            if (unRunNeighbor.second == preDirect) {//存在大概率方向相同
                nextStep = unRunNeighbor.first;
                break;
            }
        }
        if (nextStep == DIRECT_NULL) {//如果没有直接相等的方向时
            //过滤相同方向的邻居
            Calculate::deleteOppositeDirectCouples(unRunNeighbors);
            //最相近的一个方向作为前进方向（暂设方向偏差最大值4）todo
            for (int diff = 1; nextStep == DIRECT_NULL && diff < 4; diff++) {
                for (const auto& unRunNeighbor : unRunNeighbors) {
                    if ((unRunNeighbor.second + diff) % 8 == preDirect ||
                        (preDirect + diff) % 8 == unRunNeighbor.second) {//若大概率方向距离为diff
                        nextStep = unRunNeighbor.first;
                        break;
                    }
                }
            }
        }
    }

    if (nextStep != DIRECT_NULL) {//如果存在前进方向
        path.pushMove(nextStep);
        Point nextPoint = Neighbor::nextOne(now, nextStep);//下一个节点
        this->val[nextPoint.x][nextPoint.y] = color;//更新下一个节点颜色
        //继续递归
        searchNextPoint(nextPoint, path, color);
    }
}
int Calculate::findPathMainDirect(const Point& now, const vector<int>& preDirects, vector<int>& directs, const int& minCount, const int& color) {
    //下一个方向和像素
    int nextDirect = -1;
    Point nextPoint;
    //扫描周围像素
    for (int i = 0; i < 8; i++) {
        if (find(preDirects.begin(), preDirects.end(), i) == preDirects.end()) {//方向不为原先方向时
            nextPoint = Neighbor::nextOne(now, i);//下一个方向像素
            if (this->checkPoint(nextPoint) && this->val[nextPoint.x][nextPoint.y] == color) {//如果像素合法且没有被走过
                if (nextDirect != -1) {//如果有多个分支
                    nextDirect = -1;
                    break;
                }
                nextDirect = i;
            }
        }
    }
    
    if (nextDirect == -1) {//如果没有其他分支或有多个分支时退出递归
        //此时没有两次以上的方向，返回第一次方向
        return directs.front();
    }

    directs.emplace_back(nextDirect);//添加此次方向
    if (count(directs.begin(), directs.end(), nextDirect) >= minCount) {//出现大概率方向，退出递归
        return nextDirect;
    }
    return findPathMainDirect(nextPoint, { Neighbor::reverse(nextDirect) }, directs, minCount, color);//继续递归
}
void Calculate::deleteOppositeDirectCouples(map<int, int>& unRunNeighbors) {
    auto iter = unRunNeighbors.begin();
    while (iter != unRunNeighbors.end()) {//遍历邻居数组
        auto oppositeIter = find_if(unRunNeighbors.begin(), unRunNeighbors.end(),
            [iter](const pair<int, int>& i) -> bool {return Neighbor::reverse(iter->second) == i.second; });//从一点出发，需要翻转判断
        if (oppositeIter != unRunNeighbors.end()) {//找到相对方向，删除
            unRunNeighbors.erase(oppositeIter);
            unRunNeighbors.erase(iter++);
        }
        else ++iter;
    }
}

void Calculate::printMatrix() {
    ostringstream& ss = Log::ss;
    ss << "操作时矩阵：" << endl;
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            ss << this->val[i][j] << " ";
        }
        ss << endl;
    }
    ss << endl;
    Log::write();
}

void Calculate::printSourceMatrix() {
    ostringstream& ss = Log::ss;
    ss << "源图像矩阵：" << endl;
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            ss << this->source[i][j] << " ";
        }
        ss << endl;
    }
    ss << endl;
    Log::write();
}

bool Calculate::isAllPassed() {
    for (int x = 0; x < this->height; x++) {
        for (int y = 0; y < this->width; y++) {
            if (this->val[x][y] == 1) return false;
        }
    }
    return true;
}

int Calculate::isPathFinished(const Path& path, int nextStep)
{
    const vector<int>& move = path.getMove()
        , move2 = path.getMove(2)
        , move3 = path.getMove(3)
        , moveMore = path.getMove(4);
    /*
    情况一：
    横折钩，即有右、下，不能再向左
    */
    if (moveMore.size() >= 2 && nextStep == DIRECT_LEFT &&
        find(moveMore.begin(), moveMore.end(), DIRECT_RIGHT) != moveMore.end() &&
        find(moveMore.begin(), moveMore.end(), DIRECT_BOTTOM) != moveMore.end()) {
        return true;
    }
    /*
    情况二：
    闭环如口，需要第一次下后中断
    */
    //if(moveMore.size() >= 2 && )


    return false;
}

void Calculate::printPaths() {
    for (const Path& path : this->paths) {
        path.print();
    }
    Log::write("");
}

void Calculate::getRadii() {
    for (Path& path : this->paths) {
        //起始点
        Point point = path.getFromPoint();
        path.setRadius(0, this->getRadius(point));
        //其他点
        for (int i = 0; i < path.getPointCount() - 1; i++) {
            point = Neighbor::nextOne(point, path.getStep(i));
            if (!path.setRadius(i + 1, this->getRadius(point)))
                return;
        }
    }
}

float Calculate::getRadius(const Point& point) {
    //利用源图像像素矩阵计算半径
    float radius = this->width;
    /*
    方案一：
        上下左右四个方向的最小半径作为该点半径
    */
    vector<vector<int>> directs = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    for (const vector<int>& direct : directs) {//四个方向
        Point nowPoint = point;
        int nowRadius = 1;
        //计算半径
        while (Point::check(nowPoint, this->height - 1, this->width - 1) && this->source[nowPoint.x][nowPoint.y] > 0) {
            nowPoint.x += direct[0];
            nowPoint.y += direct[1];
            nowRadius++;
        }
        radius = min(radius, nowRadius);
    }
    /*
    方案二：
        追加斜方向
    */
    directs = { {-1, 1}, {-1, -1}, {1, -1}, {1, 1} };
    for (const vector<int>& direct : directs) {//四个方向
        Point nowPoint = point;
        float nowRadius = 0.7;
        //计算半径
        while (Point::check(nowPoint, this->height - 1, this->width - 1) && this->source[nowPoint.x][nowPoint.y] > 0) {
            nowPoint.x += direct[0];
            nowPoint.y += direct[1];
            nowRadius += 1.4;
        }
        radius = min(radius, nowRadius);
    }

    return radius;
}

void Calculate::saveImage()
{
    ImageFactory::output(this->val, this->source);
}

void Calculate::saveSql()
{
    MysqlFactory* factory = MysqlFactory::getInstance();
    if (!factory) {
        return;
    }
    //计算左上角坐标并存储
    Point point(-1, -1);
    string& sourcePath = BmpInfo::sourcePath;
    int fromIdx = sourcePath.find('(');
    if (fromIdx++ != string::npos) {//有位置信息
        istringstream ss(sourcePath.substr(fromIdx));//字符流提取位置
        int x, y;
        char tmp;
        ss >> x >> tmp >> y;
        point = Point(x, y);
    }
    factory->saveWordInfo(point);
    for (const Path& path : this->paths) {
        if (!factory->saveStrokeInfo(path)) {
            cout << endl << path.getFromPoint().x << "," << path.getFromPoint().y << "到" 
                << path.getToPoint().x << "," << path.getToPoint().y << " 笔画数据sql存储失败!" << endl;
        }
    }
}


