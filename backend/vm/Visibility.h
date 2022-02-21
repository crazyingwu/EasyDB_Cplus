//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_VISIBILITY_H
#define EASYDB_VISIBILITY_H

#include "../tm/TransactionManager.h"
#include "Transaction.h"
#include "Entry.h"

class Visibility{
public:
    /**
     * 检查是否有版本跳跃
     * 在可重复读的隔离环境下，版本跳跃是不允许的
     * TODO 为什么版本跳跃不能允许
     * @param tm
     * @param t
     * @param e
     * @return
     */
    static bool isVersionSkip(TransactionManager tm, Transaction t, Entry e);

    /**
     * 根据事务的不同隔离级别，来判断记录的可见性
     *
     * @param tm
     * @param t
     * @param e
     * @return
     */
    static bool isVisible(TransactionManager tm, Transaction t, Entry e);

    /**
     * 在readCommitted的隔离级别下，判断记录对当前事务是否可见
     * @param tm
     * @param t
     * @param e
     * @return
     */
    static bool readCommitted(TransactionManager tm, Transaction t, Entry e);

    /**
     * 在repeatableRead的隔离级别下，判断记录对当前事务是否可见
     * @param tm
     * @param t
     * @param e
     * @return
     */
    static bool repeatableRead(TransactionManager tm, Transaction t, Entry e);


};
#endif //EASYDB_VISIBILITY_H
