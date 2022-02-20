//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_PAGEX_H
#define EASYDB_PAGEX_H

#include "Page.h"

/*
 * 普通数据页
 * 前两个字节标识剩余空间
 * 后面的字节全是具体数据
*/
class PageX {
private:
    int OF_FREE;
    int OF_DATA;
    int MAX_FREE_SPACE;
public:
    unsigned char* initRaw();
    static void setFSO(unsigned char* raw, int free);
    static int getFSO(Page pg);
    static int getFSO(unsigned char* raw);
};


#endif //EASYDB_PAGEX_H
