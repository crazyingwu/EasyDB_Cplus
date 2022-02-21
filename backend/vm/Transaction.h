//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_TRANSACTION_H
#define EASYDB_TRANSACTION_H

#include "map"
class Transaction{
public:
    // 事务id
    long xid;
    // 事务级别
    int level;
    // 事务开始时候的活跃事务快照
    std::map<long, bool> snapshot;
    // 执行时的错误
    std::exception err;
    // TODO 是否自动回滚？怎么用的
    bool autoAborted;
    /**
     * 创建一个事务的抽象结构
     * @param xid 当前事务id
     * @param level 事务级别
     * @param active 当前活跃的所有事务，在vm中管理的，函数中会根据这个数据构建自己的快照
     * @return
     */
    static Transaction newTransaction(long xid, int level, std::map<long, Transaction> active);

    /**
     * 判断指定xid是否快照(readView)中
     * 如果xid是超级事务id，直接返回false
     * @param xid
     * @return
     */
    bool isInSnapshot(long xid);

};

#endif //EASYDB_TRANSACTION_H
