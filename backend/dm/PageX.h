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
    /*
     * 此函数在宕机后日志恢复时使用
     * page中存放的是一个个的dataItem
     * 这里直接插入到相应的位置，可能会想，后面的数据怎么办？
     * 其实完全不必担心，因为从根据日志的时间顺序，这里的数据出了问题，后面的数据肯定也有问题
     * 需要都从日志里进行恢复
     * TODO 那这里有问题：怎么判断一个日志是否需要被写入？
     * 如果重复写，显然不会出问题，但是肯定对性能有影响，不应该重复的去写
     * 功能：将raw插入到pg中的offset的位置，并将offset设置为较大的offset
     * */
    static void recoverInsert(Page pg, unsigned char* raw, short offset);


    /*
     * 将数据写入到offset处的位置去
     * TODO 这里没管offset的更新，后面看看怎么回事
     * */
    static void recoverUpdate(Page pg, unsigned char* raw, short offset);

};


#endif //EASYDB_PAGEX_H
