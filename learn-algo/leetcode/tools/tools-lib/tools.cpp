#include "tools.h"

// 大小写转换函数
void strToUpper(string &s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
}

void strToLower(string &s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
}


