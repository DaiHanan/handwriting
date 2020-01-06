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
    this->id = this->getNewWordId();
    if (this->id == -1) return false;

    string sqlStr = "INSERT INTO word ";
    sqlStr += "(id, pageId, name, locationX, locationY) ";
    sqlStr += "VALUES (" + to_string(this->id) + ", " + to_string(pageId) + ", '" + name + "', " +
        to_string(location.x) + ", " + to_string(location.y) + ") ";

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

    //ƴ��sql���
    string sqlStr = "INSERT INTO stroke ";
    sqlStr += "(wordId, startX, startY, endX, endY, path, radius) "; 
    sqlStr += "VALUES (" + to_string(this->id) + ", " + to_string(from.x) + ", " + to_string(from.y) + ", ";
    sqlStr += to_string(to.x) + ", " + to_string(to.y) + ", '" + move + "', '" + radius + "')";

    return this->update(sqlStr);
}

bool MysqlFactory::update(const string sqlStr)
{
    mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����  
    int res = mysql_query(&myCont, sqlStr.c_str());//ִ�в�����䣬mysql_query�������ɹ����㣻�������һ�����󣬷��㡣  
    return res == 0;
}

int MysqlFactory::getNewWordId()
{
    int res = mysql_query(&myCont, "SELECT id FROM word ORDER BY id DESC LIMIT 1");//ִ�в�ѯ��䣬mysql_query�����ѯ�ɹ����㣻�������һ�����󣬷��㡣  
    if (!res)
    {
        result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result  
        if (result)
        {
            if (sql_row = mysql_fetch_row(result))//��ȡ���������  
            {
                return stoi(sql_row[0]);
            }
            else {//��ǰΪ��һ������
                return 1;
            }
        }
    }
    return -1;//ʧ��
}

MysqlFactory::~MysqlFactory()
{
    mysql_close(&myCont);//�ر�MySQL����  
    mysql_library_end();//�ر�MySQL�� 
}
