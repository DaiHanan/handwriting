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
	���ʵ��
	*/
	static MysqlFactory* getInstance();
	/*
	����������Ϣ
	@param location ������ҳ�������Ͻǵ�����
	@param name ���ֵĺ�����
	@param pageId ����������ҳ��
	*/
	bool saveWordInfo(const Point& location, string name = "Ĭ������", int pageId = 1);
	/*
	����ʻ���Ϣ
	@param path �ñʻ�����Ϣ����㡢�յ㡢·�����뾶��
	*/
	bool saveStrokeInfo(const Path& path);

	~MysqlFactory();

private:
	static MysqlFactory* instance;//ʵ������

	int id;//������ı��

	const string host = "localhost";//������IP localhost 47.102.149.16
	const string user = "root";//�û���
	const string password = "liyulin";//�û�����
	const string table = "handwriting";//���ݿ���
	const unsigned int port = 3306;//���ݿ�˿ں�

    MYSQL myCont;//����MYSQL���󣬶������ݿ����Ӿ��  
    MYSQL_RES* result = NULL;//��ѯ���������Ų�ѯ���  
    MYSQL_ROW sql_row;//���һ�в�ѯ������ַ�������  
    MYSQL_FIELD* fd; // �����ֶ���Ϣ�Ľṹ

	/*
	��ʼ��ʵ��
	*/
	bool init();
	/*
	ִ�в������
	*/
	bool update(const string sqlStr);
	/*
	�����һ�����������ı�� -1ʱʧ��
	*/
	int getNewWordId();
};

