//
// Created by 40643 on 2022/2/25.
//

#ifndef EASYDB_TABLEMANAGER_H
#define EASYDB_TABLEMANAGER_H

#include "BeginRes.h"

class TableManager{
public:
    /**
     * 根据begin语句开启一个事务
     * @param begin 
     * @return 
     */
    BeginRes begin(Begin begin);
    
    unsigned char* commit(long xid);
    unsigned char* abort(long xid);

    unsigned char* show(long xid);
    /**
     * 创建一个表
     * @param xid
     * @param create
     * @return
     */
    unsigned char* create(long xid, Create create);

    /**
     * 向表中插入数据
     * @param xid
     * @param insert
     * @return
     */
    unsigned char* insert(long xid, Insert insert);

    /**
     * 读取数据
     * @param xid
     * @param select
     * @return
     */
    unsigned char* read(long xid, Select select);

    /**
     * 更新数据
     * @param xid
     * @param update
     * @return
     */
    unsigned char* update(long xid, Update update);

    /**
     * 删除数据
     * @param xid
     * @return
     */
    unsigned char* delete_(long xid, Delete delete_);
};

#endif //EASYDB_TABLEMANAGER_H
