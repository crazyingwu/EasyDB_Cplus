//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_DATAITEM_H
#define EASYDB_DATAITEM_H


#include "Page.h"

/**
 * dataItem 结构如下：
 * [ValidFlag] [DataSize] [Data]
 * ValidFlag 1字节，0为合法，1为非法
 * DataSize  2字节，标识Data的长度
 */
class DataItem {
private:
    unsigned char* raw;
    unsigned char* oldRaw;
    long uid;
    Page* pg;
public:
    // 拿到DataItem的具体数据，具体来说就是从raw中偏移前3个字节，读取剩余的全部数据
    unsigned char* data();

    // 此函数需要在修改dataItem之前调用，函数将老数据备份到oldRaw当中去
    void before();

    // 此函数在修改了dataItem之后需要调用，在此函数中会生成update日志落库，保证数据库的可靠性
    void after();

    //如果要撤销对dataItem的修改，调用此函数即可，会使用之前保存到oldRaw中的数据覆盖回实际raw当中
    void unBefore();

    // 释放掉当前dataItem的缓存
    /*
     * 1.dataItem也有缓存，在dataManager当中，dataManager实现了AbstractCache类
     * 2.dataManager当中的数据单元就是dataItem，读取数据时，如果缓存没命中，就是从page当中去加载
     * 3.加载的uid有一套规则，因为我们需要通过一个key，同时得到页号和偏移，所以key中需要这两个信息
     * */
    void release();

    //返回当前所在page，这里的page是在构造函数当中传入的
    Page* page();


    // 通过读取第一个字节（标识位），来判断当前数据是否有效
    bool isValid();

    unsigned char* getRaw();
    unsigned char* getOldRaw();


};


#endif //EASYDB_DATAITEM_H
