//
// Created by 40643 on 2022/2/20.
//

#ifndef EASYDB_PAGEONE_H
#define EASYDB_PAGEONE_H
#include "Page.h"

class PageOne {
private:
    static int OF_VC;
    static int LEN_VC;

public:
    // 初始化第一页的数据
    static unsigned char* initRaw();
    // 打开时生成校验码
    static void setVcOpen(Page pg);
    static void setVcOpen(unsigned char* raw);
    //关闭时复制之前的校验码
    static void setVcClose(Page pg);
    static void setVcClose(unsigned char* raw);
    // 进行校验
    static bool checkVc(Page pg);
};


#endif //EASYDB_PAGEONE_H
