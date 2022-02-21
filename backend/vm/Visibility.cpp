//
// Created by 40643 on 2022/2/21.
//
#include "Visibility.h"

bool Visibility::isVisible(TransactionManager tm, Transaction t, Entry e) {
    if (t.level == 0) {
        readCommitted(tm, t, e);
    } else{
        repeatableRead(tm, t, e);
    }
}

bool Visibility::readCommitted(TransactionManager tm, Transaction t, Entry e) {
    long xid = t.xid;
    long xmin = e.getXMin();
    long xmax = e.getXMax();
    // 如果这条记录的该版本就是本事务创建的，且还没被删除，则可见
    if (xmin == xid && xmax == 0)
        return true;
    if (tm.isCommitted(xmin)) {
        // 创建该记录的事务已提交，且记录未被删除，则可见
        if (xmax == 0) return true;
        if (xmax != xid) {
            // 即将删除该记录的事务还未提交，则也应该可见
            if (!tm.isCommitted(xmax)) {
                return true;
            }
        }
    }
    return false;
}

bool Visibility::repeatableRead(TransactionManager tm, Transaction t, Entry e) {
    long xid = t.xid;
    long xmin = e.getXMin();
    long xmax = e.getXMax();
    // 如果这条记录的该版本就是本事务创建的，且还没被删除，则可见
    if (xmin == xid && xmax == 0)
        return true;
    /*
     * 三个条件
     * 1.创建该记录的事务已提交
     * 2.当前事务晚于创建的事务开始
     * 3.创建的事务在本事务开始的时候不处于活跃状态
     * */
    if (tm.isCommitted(xmin) && xmin < xid && !t.isInSnapshot(xmin)) {
        // 满足前面的条件后，记录也没被某个事务删除，则可见
        if (xmax == 0) return true;
        // 保证自己之前没有删掉这个记录
        if (xmax != xid) {
            /*
             * 如果有事务要删除这条记录，则满足下面三个条件中的一个，记录仍然可见
             * 1.要删除的事务还没有提交
             * 2.要删除的事务晚于当前开始的事务
             * 3.当前事务开始的时候，删除事务仍然活跃(未提交)，也就是生成的快照中还有要删除事务的信息
             * */
            if (!tm.isCommitted(xmax) || xmax > xid || t.isInSnapshot(xmax)) {
                return true;
            }
        }
    }
    return false;
}