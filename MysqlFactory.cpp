#include "MysqlFactory.h"

MysqlFactory* MysqlFactory::instance = nullptr;

MysqlFactory* MysqlFactory::getInstance()
{
	if (MysqlFactory::instance == nullptr) {
		MysqlFactory::instance = new MysqlFactory();
		if (!instance->init()) {
			return nullptr;
		}
	}
	return MysqlFactory::instance;
}

bool MysqlFactory::init()
{
	mysql_library_init(0, NULL, NULL);//��ʼ��MySQL��  
	mysql_init(&myCont);//��ʼ�����Ӵ������  
	return mysql_real_connect(&myCont, host.c_str(), user.c_str(), password.c_str(), table.c_str(), port, NULL, 0);
}

bool MysqlFactory::saveWordInfo(const Point& location, string name, int pageId)
{
    string sqlStr = "INSERT INTO word ";
    sqlStr += "(pageId, name, locationX, locationY) ";
    sqlStr += "VALUES (" + to_string(pageId) + ", '" + name + "', " + 
        to_string(location.x) + ", " + to_string(location.y) + ") ";

    //cout << sqlStr << endl;
    return this->update(sqlStr);
}

bool MysqlFactory::saveStrokeInfo(const Path& path)
{
    //��㡢�յ�
    const Point& from = path.getFromPoint(), &to = path.getToPoint();

    ostringstream ss;
    //·��
    for (int i = 0; i < path.getPointCount() - 1; i++) {
        ss << path.getStep(i) << ",";
    }
    string move = ss.str();
    move = move.substr(0, move.size() - 1);//ɾ����󶺺�
    //�뾶
    ss.str("");//���
    for (int i = 0; i < path.getPointCount(); i++) {
        ss << path.getRadius(i) << ",";
    }
    string radius = ss.str();
    radius = radius.substr(0, radius.size() - 1);//ɾ����󶺺�
    //todo �洢
    return false;
}

bool MysqlFactory::update(const string sqlStr)
{
    mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����  
    int res = mysql_query(&myCont, sqlStr.c_str());//ִ�в�����䣬mysql_query�������ɹ����㣻�������һ�����󣬷��㡣  
    return res != 0;
}

MysqlFactory::~MysqlFactory()
{
    mysql_close(&myCont);//�ر�MySQL����  
    mysql_library_end();//�ر�MySQL�� 
}
