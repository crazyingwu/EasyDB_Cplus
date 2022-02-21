//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_VERSIONMANAGER_H
#define EASYDB_VERSIONMANAGER_H


#include "../tm/TransactionManager.h"
#include "../dm/DataManager.h"
#include "Transaction.h"
#include "Entry.h"
#include "LockTable.h"

class VersionManager : AbstractCache<Entry>{

private:
    TransactionManager tm;
    DataManager dataManager;
    // 记录了当前所有的活动事务id
    std::map<long, Transaction> active;

    std::mutex mutex;

    LockTable lockTable;

public:
    /**
     * 读取一条entry
     * @param xid
     * @param uid
     * @return
     */
    unsigned char* read(long xid, long uid);

    /**
     * 插入一条entry
     * @param xid
     * @param data
     * @return
     */
    long insert(long xid, unsigned char* data);

    /**
     * 删除一条entry
     * @param xid
     * @param uid
     * @return
     */
    bool delete_(long xid, long uid);

    /**
     * 开启一个事务
     * @param level
     * @return
     */
    long begin(int level);

    /**
     * 提交一个事务
     * @param xid
     */
    void commit(long xid);

    /**
     * 回滚一个事务
     * @param xid
     */
    void abort(long xid);

    void internAbort(long xid, bool b);
};

#endif //EASYDB_VERSIONMANAGER_H
