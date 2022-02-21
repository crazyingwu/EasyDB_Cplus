//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_PAGEINDEX_H
#define EASYDB_PAGEINDEX_H
#include "mutex"
#include "vector"
#include "PageInfo.h"

class PageIndex{
private:
    // 区间间隔
    int INTERVALS_NO;
    // 空间单位
    int THRESHOLD;
    // 读写index的锁
    std::mutex mutex;
    // 存放了全部的页面信息的列表，分为了40个区间，每个区间内的page剩余空间能满足一定大小的空间需求
    std::vector<PageInfo>* lists;

public:
    /*
     * 1. 通过剩余空间计算应该在哪个区间
     * int number = freeSpace / THRESHOLD;
     * 2. 在指定区间的列表里添加上pageInfo
     * lists[number].add(new PageInfo(pgno, freeSpace));
     * */
    void add(int pgno, int freeSpace);

    /*
     * 1. 根据需要空间的大小计算应该从哪个区间取页面
     * 2. 如果计算得到的区间内没有取到页面，则向下继续寻找，即往更大剩余空间的区间寻找
     * 3. 拿到页面后需要将页面从pageIndex列表中删除，防止对一个页面的并发读写、写写
     * */
    PageInfo select(int spaceSize);

};


#endif //EASYDB_PAGEINDEX_H
