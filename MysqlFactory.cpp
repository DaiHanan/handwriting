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
	mysql_library_init(0, NULL, NULL);//初始化MySQL库  
	mysql_init(&myCont);//初始化连接处理程序  
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
    //起点、终点
    const Point& from = path.getFromPoint(), &to = path.getToPoint();

    ostringstream ss;
    //路径
    for (int i = 0; i < path.getPointCount() - 1; i++) {
        ss << path.getStep(i) << ",";
    }
    string move = ss.str();
    move = move.substr(0, move.size() - 1);//删除最后逗号
    //半径
    ss.str("");//清空
    for (int i = 0; i < path.getPointCount(); i++) {
        ss << path.getRadius(i) << ",";
    }
    string radius = ss.str();
    radius = radius.substr(0, radius.size() - 1);//删除最后逗号

    //拼接sql语句
    string sqlStr = "INSERT INTO stroke ";
    sqlStr += "(wordId, startX, startY, endX, endY, path, radius) "; 
    sqlStr += "VALUES (" + to_string(this->id) + ", " + to_string(from.x) + ", " + to_string(from.y) + ", ";
    sqlStr += to_string(to.x) + ", " + to_string(to.y) + ", '" + move + "', '" + radius + "')";

    return this->update(sqlStr);
}

bool MysqlFactory::update(const string sqlStr)
{
    mysql_query(&myCont, "SET NAMES GBK"); //设置编码格式,否则在cmd下无法显示中文  
    int res = mysql_query(&myCont, sqlStr.c_str());//执行插入语句，mysql_query如果插入成功，零；如果出现一个错误，非零。  
    return res == 0;
}

int MysqlFactory::getNewWordId()
{
    int res = mysql_query(&myCont, "SELECT id FROM word ORDER BY id DESC LIMIT 1");//执行查询语句，mysql_query如果查询成功，零；如果出现一个错误，非零。  
    if (!res)
    {
        result = mysql_store_result(&myCont);//保存查询到的数据到result  
        if (result)
        {
            if (sql_row = mysql_fetch_row(result))//获取具体的数据  
            {
                return stoi(sql_row[0]);
            }
            else {//当前为第一个数据
                return 1;
            }
        }
    }
    return -1;//失败
}

MysqlFactory::~MysqlFactory()
{
    mysql_close(&myCont);//关闭MySQL连接  
    mysql_library_end();//关闭MySQL库 
}
