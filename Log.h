#pragma once

#define LOG_PATH "D:/01_files/05_upward/01_projects/09_VisualStudioPro/handwriting/log.txt"

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Log {
public:
    static ostringstream ss;//�ַ���
    /**
     * ��ӡ
     * @param str ��Ҫ��ӡ���ַ���
     */
    static void write(string str);
    /**
     * ��ӡ����
     * ʹ���ַ���
     */
    static void write();
private:
    static ofstream os;//�����
};

