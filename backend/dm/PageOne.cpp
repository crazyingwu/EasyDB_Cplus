//
// Created by 40643 on 2022/2/20.
//

#include "PageOne.h"
#define PAGE_SIZE 100

unsigned char *PageOne::initRaw() {
    // 首先生成一个新页的数据
    unsigned char * data = new unsigned char [PAGE_SIZE];
    setVcOpen(data);
    return data;
}

void PageOne::setVcOpen(Page pg) {
    pg.setDirty(true);
    setVcOpen(pg.getData());
}

void PageOne::setVcOpen(unsigned char *raw) {
    // 随机生成指定长度的字节
    unsigned char* data = new unsigned char[LEN_VC];
    //TODO 将生成的data替换到raw中的指定位置当中去
    //Java代码如下：
    // System.arraycopy(RandomUtil.randomBytes(LEN_VC), 0, raw, OF_VC, LEN_VC);
}

void PageOne::setVcClose(Page pg) {
    pg.setDirty(true);
    setVcClose(pg.getData());
}

void PageOne::setVcClose(unsigned char *raw) {
    // TODO 将指定偏移的数据复制到指定偏移当中去
    // System.arraycopy(raw, OF_VC, raw, OF_VC+LEN_VC, LEN_VC);
}

bool PageOne::checkVc(Page pg) {
    //TODO 比较文件中指定偏移的两个地方的数据是否相同
}