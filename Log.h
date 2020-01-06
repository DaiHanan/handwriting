#pragma once

#define LOG_PATH "D:/01_files/05_upward/01_projects/09_VisualStudioPro/handwriting/log.txt"

#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Log {
public:
    static ostringstream ss;//字符流
    /**
     * 打印
     * @param str 需要打印的字符串
     */
    static void write(string str);
    /**
     * 打印重载
     * 使用字符流
     */
    static void write();
private:
    static ofstream os;//输出流
};

