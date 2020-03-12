#pragma once
#include "Point.h"
#include <string>
#include <winsock.h>
#include <mysql.h>
#include <iostream>
#include "Path.h"
#include <sstream>

using namespace std;


class MysqlFactory
{
public:
	/*
	获得实例
	*/
	static MysqlFactory* getInstance();
	/*
	保存字体信息
	@param location 该字在页面中左上角的坐标
	@param name 该字的汉字名
	@param pageId 该字所属的页数
	*/
	bool saveWordInfo(const Point& location, string name = "默认名称", int pageId = 1);
	/*
	保存笔画信息
	@param path 该笔画的信息（起点、终点、路径、半径）
	*/
	bool saveStrokeInfo(const Path& path);

	~MysqlFactory();

private:
	static MysqlFactory* instance;//实例对象

	int id;//该字体的编号

	const string host = "localhost";//服务器IP localhost 47.102.149.16
	const string user = "root";//用户名
	const string password = "liyulin";//用户密码
	const string table = "handwriting";//数据库名
	const unsigned int port = 3306;//数据库端口号

    MYSQL myCont;//创建MYSQL对象，定义数据库连接句柄  
    MYSQL_RES* result = NULL;//查询结果集，存放查询结果  
    MYSQL_ROW sql_row;//存放一行查询结果的字符串数组  
    MYSQL_FIELD* fd; // 包含字段信息的结构

	/*
	初始化实例
	*/
	bool init();
	/*
	执行插入操作
	*/
	bool update(const string sqlStr);
	/*
	获得下一个插入的字体的编号 -1时失败
	*/
	int getNewWordId();
};

