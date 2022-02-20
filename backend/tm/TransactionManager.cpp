//
// Created by 40643 on 2022/2/20.
//

#include "TransactionManager.h"

long TransactionManager::begin() {
    long xid = xidCounter + 1;
    updateXID(xid, FIELD_TRAN_ACTIVE);
    xidCounter++;
    return xid;
}

void TransactionManager::updateXID(long xid, unsigned char active) {
    // 获取对应事务数据的偏移
    long offset = getXidPosition(xid);
    //TODO 对文件从指定位置进行读写

}

long TransactionManager::getXidPosition(long xid) {
    return 0;
}
