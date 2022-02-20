//
// Created by 40643 on 2022/2/21.
//

#include "PageX.h"

unsigned char *PageX::initRaw() {
    // 新建一个新的页的数据
    unsigned char* raw = new unsigned char[100];
    // 初始化时，空闲指针就紧跟着空闲数量的后面，也就是第2个字节完
    setFSO(raw, OF_DATA);
    return raw;
}

void PageX::setFSO(unsigned char *raw, int free) {
    // TODO 把给定的free写入到前两个字节即可
}

int PageX::getFSO(unsigned char *raw) {
    // TODO 取出前两个字节，转为整数返回
}

int PageX::getFSO(Page pg) {
    return PageX::getFSO(pg.getData());
}