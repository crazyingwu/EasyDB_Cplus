//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_DATAMANAGER_H
#define EASYDB_DATAMANAGER_H

#include "DataItem.h"
#include "../tm/TransactionManager.h"
#include "PageOne.h"

class DataManager {
private:
    /*
     * DataManager中需要tm的支持，主要体现在：
     * 发现好像不需要，我去，多余的？
    */
    TransactionManager tm;
    PageCache pageCache;
    // TODO 还需要引入日志记录器，pageIndex
    PageOne pageOne;

public:
    // 根据uid读取出dataItem来，并判断dataItem.isValid()，即dataItem是否有效
    DataItem read(long uid);

    //根据数据生成一条dataItem并寻找page插入
    /*
     * 这里的插入过程会使用到pageIndex
     * 1. 从pageIndex中寻找到满足空间的page，没找到会生成新页加入到索引中再找
     * 找到之后根据xid和data生成一个dataItem
     * 2. 先写入日志
     * 3. 调用pageX.insert()来将数据实际插入到选定的页面
     * */
    long insert(long xid, unsigned char* data);

    // 根据文件路径和缓存占用的最大内存来生成DataManager
    // 这里的文件主要是PageCache来创建页面文件，然后限制pageCache的最大缓存内存
    // 在create中调用pageCache.create(path, memory)
    // 然后会初始化pageOne，也就是第一页，生成校验码
    static DataManager create(std::string path, long memory);


    /*
     * 从现有文件初始化DataManager
     * 主要是pageCache从现有文件进行打开
     * 然后DataManager校验第一页的数据，检查上一次数据库是否正常关闭
     * 然后没有正常关闭，就需要执行恢复步骤，根据日志来进行恢复
     * 恢复完后，在本函数中还需要进行以下工作：
     * 1. 建立页面的索引，也就是遍历页面，取出空闲空间，挂到40个空闲区间中的列表去
     * 2. 重新为第一页生成新的校验码，并且马上通过flushPage刷新到磁盘去
     * */
    static DataManager open(std::string path, long memory);
};


#endif //EASYDB_DATAMANAGER_H
