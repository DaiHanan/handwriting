#include "Log.h"

ofstream Log::os;
ostringstream Log::ss;

void Log::write(string str) {
    Log::os.open(LOG_PATH, ios::app);
    os << str << endl;
    os.close();
}

void Log::write() {
    Log::write(Log::ss.str());
    ss.str("");//清空字符流，下次使用
}