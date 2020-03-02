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
    //this->saveSql();//���н�������Ϣ�������ݿ⣬ȡ���������ظ�����,to�����
}

void Calculate::toThin() {
    vector<vector<int>>& vals = this->val;
    int rows[] = { 0, -1, 0, 1 };//i ��������
    int cols[] = { 1, 0, -1, 0 };//j
    for (int i = 1; i < vals.size() - 1; i++) {
        for (int j = 1; j < vals[0].size() - 1; j++) {
            if (vals[i][j] == 1) {//��ɫ
                int count = 0;
                for (int k = 0; k < 4; k++) {//�ĸ�����
                    if (vals[i + rows[k]][j + cols[k]] == 1)
                        count++;
                }
                if (count == 2) {//����
                    //�������
                    if (
                        (vals[i + rows[0]][j + cols[0]] == 1 && vals[i + rows[1]][j + cols[1]] == 1) ||//����
                        (vals[i + rows[1]][j + cols[1]] == 1 && vals[i + rows[2]][j + cols[2]] == 1) ||//����
                        (vals[i + rows[2]][j + cols[2]] == 1 && vals[i + rows[3]][j + cols[3]] == 1) ||//����
                        (vals[i + rows[3]][j + cols[3]] == 1 && vals[i + rows[0]][j + cols[0]] == 1) //����
                        )
                        vals[i][j] = 0;
                }
                else if (count == 3) {//͹��
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
    //�˽����±�
    int(&xx)[8] = Neighbor::x;
    int(&yy)[8] = Neighbor::y;
    //Ѱ�Ҷ˵�
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            //��ÿ����ɫ���ص����
            if (this->val[i][j] == 1) {
                int neighbourCount = 0;//�ھ�����
                for (int k = 0; k < 8; k++) {
                    //�˽��ڵ�������ͺ�����
                    int x = i + xx[k], y = j + yy[k];
                    if (this->checkPoint(Point(x, y))
                        && this->val[x][y] == 1
                        && ++neighbourCount >= 2)
                        break;//����2�����ټ���
                }
                if (neighbourCount == 1)//ֻ��һ���ھ���Ϊ�˵�
                    _points.emplace_back(Point(i, j));
            }
        }
    }
    //���û�ж˵�ʱ�������Բ��ѭ���������ҵ�һ��δ�����㷵��
    if (_points.empty()) {
        for (int j = 0; j < this->width; j++) {
            for (int i = 0; i < this->height; i++) {
                //��ÿ����ɫ���ص����
                if (this->val[i][j] == 1) {//δ������
                    _points.emplace_back(Point(i, j));
                    return;
                }
            }
        }
    }

}

void Calculate::printPoints() {
    ostringstream& ss = Log::ss;
    ss << "�˵㼯��Ϊ��" << endl;
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
    //    if(abs(point1.y - point2.y) < 10) {//��������ʱ�Ƚ�����
    //        return point1.x < point2.x;
    //    } else return point1.y < point2.y;//�ȽϺ���
    return point1.x + point1.y < point2.x + point2.y;
}

void Calculate::getPath() {
    int color = 1;//·����ɫ
    //ȡ�����ж˵㣬ÿȡ��һ��·����ɾ���ѱ����˵���������������˵�
    vector<Point> points = this->points;
    while (!points.empty()) {
        for (int i = 0; i < points.size(); i++) {
            const Point& point = points[i];
            if (this->val[point.x][point.y] == 1) {//���֮ǰ��·��û�о����ö˵�
                this->val[point.x][point.y] = ++color;//���½ڵ���ɫ
                Path path(point);
                this->searchNextPoint(point, path, color);//�ݹ���·��
                this->paths.emplace_back(path);//�ҵ������·������
                //ȡ��һ��·������������������˵�
                points.erase(points.begin() + i);
                this->getPoints(points);
                this->sortPoints(points);
                break;
            }
            else {//����ͨ��ʱֱ��ɾ��
                points.erase(points.begin() + i--);
            }
        }
    }
    for (const Point& point : points) {
        if (this->val[point.x][point.y] == 1) {//���֮ǰ��·��û�о����ö˵�
            this->val[point.x][point.y] = ++color;//���½ڵ���ɫ
            Path path(point);
            this->searchNextPoint(point, path, color);//�ݹ���·��

            this->paths.emplace_back(path);//����·������
        }
    }
    /**
     * ��ͼ������δ�����㣬����ѭ��
     */
    while (!this->isAllPassed()) {
        //this->printMatrix();
        //Ѱ���Ӷ˵�
        vector<Point> subPoints;
        this->getPoints(subPoints);
        //���������Ӷ˵�
        for (const Point& point : subPoints) {
            if (this->val[point.x][point.y] == 1) {//���֮ǰ��·��û�о����ö˵�
                this->val[point.x][point.y] = ++color;//���½ڵ���ɫ
                Path path(point);
                this->searchNextPoint(point, path, color);//�ݹ���·��

                this->paths.emplace_back(path);//����·������
            }
        }
    }
}
void Calculate::searchNextPoint(const Point& now, Path& path, int color) {
    //�ҵ�����û���߹����ھ�
    //map: first ���ھ��������� second ������Ĵ���ʷ���
    map<int, int> unRunNeighbors;
    for (int i = 0; i < 8; i++) {
        Point nextPoint = Neighbor::nextOne(now, i);
        int x = nextPoint.x, y = nextPoint.y;
        if (this->checkPoint(nextPoint) && this->val[x][y] == 1) {
            unRunNeighbors[i] = DIRECT_NULL;//�洢����
        }
    }
    //�ݹ��ж�
    if (unRunNeighbors.empty()) return;//û�п�ǰ�����ھ�
    //��һ��
    int nextStep = DIRECT_NULL;
    if (unRunNeighbors.size() == 1) {//������֧
        nextStep = unRunNeighbors.begin()->first;
        int backCount = this->isPathFinished(path, nextStep);//���˲���
        if (backCount > -1) {//��ǰ����·��
            Point backPoint = now;
            for (int i = 0; i < backCount; i++) {
                //��������color
                this->val[backPoint.x][backPoint.y] = 1;
                //����
                Neighbor::back(backPoint, path.getStep(-1));
                path.popMove();
            }
            return;
        }
    }
    else {//�����֧
        vector<int> lengths;//ÿ����֧����������ʷ���ĳ���
        //Ѱ��ÿ����֧�Ĵ���ʷ���
        for (auto& unRunNeighbor : unRunNeighbors) {
            /**
             * ��Ϊ�Ӱ˽��ڳ��������Բ���Ҫ����������پ�����������
             * ��Ҫ�������ڷ������ǰ�÷���
             * �����ˮƽ����ֱ����Ҫ��Ӹ÷����Լ����Ҹ�2������
             * ����ǶԽǷ�����Ҫ��Ӹ÷����Լ����Ҹ�1������
             */
            vector<int> preDirects = { Neighbor::reverse(unRunNeighbor.first) };
            preDirects.emplace_back(Neighbor::reverse((unRunNeighbor.first + 1) % 8));
            preDirects.emplace_back(Neighbor::reverse((unRunNeighbor.first - 1 + 8) % 8));
            if (Neighbor::isLevel(unRunNeighbor.first)) {//ˮƽ��������2��
                preDirects.emplace_back(Neighbor::reverse((unRunNeighbor.first + 2) % 8));
                preDirects.emplace_back(Neighbor::reverse((unRunNeighbor.first - 2 + 8) % 8));
            }
            vector<int> directs(1, unRunNeighbor.first);//�ѵݹ鷽������
            unRunNeighbor.second = this->findPathMainDirect(//�������ʷ���
                Neighbor::nextOne(now, unRunNeighbor.first), preDirects, directs
            );
            lengths.emplace_back(directs.size());//���³���
        }
        /**
         * ���˵�����������·��
         * �����������ȵ���1�����ڷ������·���ҳ��Ⱦ�����1
         */
        if (lengths.size() > 2) {
            int targetIdx = 0;
            //�ҵ���Ҫ����·�����±�
            while (
                lengths.size() > 2 &&//�������ڷ���
                (targetIdx = find(lengths.begin() + targetIdx, lengths.end(), 1) - lengths.begin()) < lengths.size() &&//��·������Ϊ1
                lengths[(targetIdx + 1) % lengths.size()] > 1 &&//��
                lengths[(targetIdx - 1 + lengths.size()) % lengths.size()] > 1//��
                ) {
                //���ж��Ƿ�Ϊ·����������
                auto targetIter = unRunNeighbors.begin();//Ŀ��
                for (int i = 0; i < targetIdx; i++) {
                    ++targetIter;
                }
                int targetDirect = targetIter->first;//Ŀ�귽��
                //�ж����ڷ����Ƿ����
                if (
                    unRunNeighbors.find((targetDirect + 1) % 8) != unRunNeighbors.end() && //��
                    unRunNeighbors.find((targetDirect - 1 + 8) % 8) != unRunNeighbors.end() //��
                    ) {
                    //���ڡ�ɾ���÷���
                    unRunNeighbors.erase(targetIter);
                    lengths.erase(lengths.begin() + targetIdx);
                }
            }
        }
        /**
         * ��move���鲻Ϊ��ʱ��Ϊ�˵�
         * һ�������Ѿ�������·���Ĵ���ʷ���
         * ��move����Ϊ��ʱ��ΪԲ��ѭ����
         * ���ݹ��ɣ�Բ�γ��������ʷ�������
         */
        int preDirect = DIRECT_NULL;
        const vector<int>& move = path.getMove();
        if (!move.empty()) {
            //Ѱ���Ѿ�������·���Ĵ���ʷ���
            vector<int> directs(1, Neighbor::reverse(move.back()));//�ѵݹ鷽������
            preDirect = findPathMainDirect(Neighbor::backOne(now, move.back()), { move.back() }, directs, color);
            preDirect = Neighbor::reverse(preDirect);//��ת������Ϊ�Ƿ����ƶϷ���
        }
        else {
            preDirect = DIRECT_BOTTOM;
        }

        //Ѱ��ǰ������
        //Ѱ�ҵ�һ�����Ѿ���·������ʷ�����ͬ�ķ������ǰ��
        for (auto& unRunNeighbor : unRunNeighbors) {
            if (unRunNeighbor.second == preDirect) {//���ڴ���ʷ�����ͬ
                nextStep = unRunNeighbor.first;
                break;
            }
        }
        if (nextStep == DIRECT_NULL) {//���û��ֱ����ȵķ���ʱ
            //������ͬ������ھ�
            Calculate::deleteOppositeDirectCouples(unRunNeighbors);
            //�������һ��������Ϊǰ���������跽��ƫ�����ֵ4��todo
            for (int diff = 1; nextStep == DIRECT_NULL && diff < 4; diff++) {
                for (const auto& unRunNeighbor : unRunNeighbors) {
                    if ((unRunNeighbor.second + diff) % 8 == preDirect ||
                        (preDirect + diff) % 8 == unRunNeighbor.second) {//������ʷ������Ϊdiff
                        nextStep = unRunNeighbor.first;
                        break;
                    }
                }
            }
        }
    }

    if (nextStep != DIRECT_NULL) {//�������ǰ������
        path.pushMove(nextStep);
        Point nextPoint = Neighbor::nextOne(now, nextStep);//��һ���ڵ�
        this->val[nextPoint.x][nextPoint.y] = color;//������һ���ڵ���ɫ
        //�����ݹ�
        searchNextPoint(nextPoint, path, color);
    }
}
int Calculate::findPathMainDirect(const Point& now, const vector<int>& preDirects, vector<int>& directs, const int& minCount, const int& color) {
    //��һ�����������
    int nextDirect = -1;
    Point nextPoint;
    //ɨ����Χ����
    for (int i = 0; i < 8; i++) {
        if (find(preDirects.begin(), preDirects.end(), i) == preDirects.end()) {//����Ϊԭ�ȷ���ʱ
            nextPoint = Neighbor::nextOne(now, i);//��һ����������
            if (this->checkPoint(nextPoint) && this->val[nextPoint.x][nextPoint.y] == color) {//������غϷ���û�б��߹�
                if (nextDirect != -1) {//����ж����֧
                    nextDirect = -1;
                    break;
                }
                nextDirect = i;
            }
        }
    }
    
    if (nextDirect == -1) {//���û��������֧���ж����֧ʱ�˳��ݹ�
        //��ʱû���������ϵķ��򣬷��ص�һ�η���
        return directs.front();
    }

    directs.emplace_back(nextDirect);//��Ӵ˴η���
    if (count(directs.begin(), directs.end(), nextDirect) >= minCount) {//���ִ���ʷ����˳��ݹ�
        return nextDirect;
    }
    return findPathMainDirect(nextPoint, { Neighbor::reverse(nextDirect) }, directs, minCount, color);//�����ݹ�
}
void Calculate::deleteOppositeDirectCouples(map<int, int>& unRunNeighbors) {
    auto iter = unRunNeighbors.begin();
    while (iter != unRunNeighbors.end()) {//�����ھ�����
        auto oppositeIter = find_if(unRunNeighbors.begin(), unRunNeighbors.end(),
            [iter](const pair<int, int>& i) -> bool {return Neighbor::reverse(iter->second) == i.second; });//��һ���������Ҫ��ת�ж�
        if (oppositeIter != unRunNeighbors.end()) {//�ҵ���Է���ɾ��
            unRunNeighbors.erase(oppositeIter);
            unRunNeighbors.erase(iter++);
        }
        else ++iter;
    }
}

void Calculate::printMatrix() {
    ostringstream& ss = Log::ss;
    ss << "����ʱ����" << endl;
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
    ss << "Դͼ�����" << endl;
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
    ���һ��
    ���۹��������ҡ��£�����������
    */
    if (moveMore.size() >= 2 && nextStep == DIRECT_LEFT &&
        find(moveMore.begin(), moveMore.end(), DIRECT_RIGHT) != moveMore.end() &&
        find(moveMore.begin(), moveMore.end(), DIRECT_BOTTOM) != moveMore.end()) {
        return true;
    }
    /*
    �������
    �ջ���ڣ���Ҫ��һ���º��ж�
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
        //��ʼ��
        Point point = path.getFromPoint();
        path.setRadius(0, this->getRadius(point));
        //������
        for (int i = 0; i < path.getPointCount() - 1; i++) {
            point = Neighbor::nextOne(point, path.getStep(i));
            if (!path.setRadius(i + 1, this->getRadius(point)))
                return;
        }
    }
}

float Calculate::getRadius(const Point& point) {
    //����Դͼ�����ؾ������뾶
    float radius = this->width;
    /*
    ����һ��
        ���������ĸ��������С�뾶��Ϊ�õ�뾶
    */
    vector<vector<int>> directs = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    for (const vector<int>& direct : directs) {//�ĸ�����
        Point nowPoint = point;
        int nowRadius = 1;
        //����뾶
        while (Point::check(nowPoint, this->height - 1, this->width - 1) && this->source[nowPoint.x][nowPoint.y] > 0) {
            nowPoint.x += direct[0];
            nowPoint.y += direct[1];
            nowRadius++;
        }
        radius = min(radius, nowRadius);
    }
    /*
    ��������
        ׷��б����
    */
    directs = { {-1, 1}, {-1, -1}, {1, -1}, {1, 1} };
    for (const vector<int>& direct : directs) {//�ĸ�����
        Point nowPoint = point;
        float nowRadius = 0.7;
        //����뾶
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
    //�������Ͻ����겢�洢
    Point point(-1, -1);
    string& sourcePath = BmpInfo::sourcePath;
    int fromIdx = sourcePath.find('(');
    if (fromIdx++ != string::npos) {//��λ����Ϣ
        istringstream ss(sourcePath.substr(fromIdx));//�ַ�����ȡλ��
        int x, y;
        char tmp;
        ss >> x >> tmp >> y;
        point = Point(x, y);
    }
    factory->saveWordInfo(point);
    for (const Path& path : this->paths) {
        if (!factory->saveStrokeInfo(path)) {
            cout << endl << path.getFromPoint().x << "," << path.getFromPoint().y << "��" 
                << path.getToPoint().x << "," << path.getToPoint().y << " �ʻ�����sql�洢ʧ��!" << endl;
        }
    }
}


