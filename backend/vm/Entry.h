//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_ENTRY_H
#define EASYDB_ENTRY_H

#include "../dm/DataItem.h"
#include "VersionManager.h"

/**
 * TODO 搞明白那个subArray到底是干什么
 * vm向上层提供的数据实体
 * 格式为：[XMIN] [XMAX] [DATA]
 * XMIN: 创建该记录（版本）的事务编号
 * XMAX: 删除该记录(版本)的事务编号
 * DATA: 记录持有的数据
 */
class Entry{
private:
    long uid;
    DataItem dataItem;
    VersionManager vm;

public:
    /**
     * 创建一个新的entry
     * @param vm entry中需要持有vm的引用
     * @param dataItem 实际的数据
     * @param uid
     * @return
     */
    static Entry newEntry(VersionManager vm, DataItem dataItem, long uid);

    /**
     * 根据uid，读取得到dataItem，然后生成一个新的Entry
     * @param vm
     * @param uid
     * @return
     */
    static Entry loadEntry(VersionManager vm, long uid);


    /**
     * 包装数据为Entry的数据，即加上XMIN,XMAX
     * @param xid
     * @param data
     * @return
     */
    static unsigned char* wrapEntryRaw(long xid, unsigned char* data);

    /**
     * 释放当前entry的缓存
     * 此函数会让dataItem.release()同步data回磁盘
     * */
    void release();

    /**释放dateItem*/
    void remove();

    /**
     * 返回entry中的具体数据，具体来说就是调用dataItem.data()，然后取数据偏移后的数据
     * @return dataItem中的具体数据
     */
    unsigned char* data();

    /**
     * 拿到dataItem中的数据，取出XMin，就在前8字节
     * @return
     */
    long getXMin();

    /**
     * 拿到dataItem中的数据，取出XMax，就在8-16字节
     * @return
     */
    long getXMax();

    /**
     * 写入xid到dataItem中的对应位置
     * @param xid
     */
    void setXMax(long xid);

};

#endif //EASYDB_ENTRY_H
