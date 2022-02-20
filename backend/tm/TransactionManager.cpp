//
// Created by 40643 on 2022/2/20.
//

#include "TransactionManager.h"

TransactionManager* TransactionManager::create(std::string path) {
    // 1、根据path创建文件

    // 2、写入空文件头，即0

    return new TransactionManager();
}

TransactionManager* TransactionManager::open(std::string path) {
    // 1、根据path打开文件

    // 2、检验文件的合法性，用文件头和文件实际长度做比较
    return new TransactionManager();
}

long TransactionManager::begin() {
    long xid = xidCounter + 1;
    updateXID(xid, FIELD_TRAN_ACTIVE);
    xidCounter++;
    return xid;
}

void TransactionManager::commit(long xid) {
    updateXID(xid, FIELD_TRAN_COMMITTED);
}

void TransactionManager::abort(long xid) {
    updateXID(xid, FIELD_TRAN_ABORTED);
}

bool TransactionManager::isActive(long xid) {
    return checkStatus(xid, FIELD_TRAN_ACTIVE);
}

bool TransactionManager::isCommitted(long xid) {
    return checkStatus(xid, FIELD_TRAN_COMMITTED);
}

bool TransactionManager::isAborted(long xid) {
    return checkStatus(xid, FIELD_TRAN_ABORTED);
}

void TransactionManager::updateXID(long xid, unsigned char active) {
    // 获取对应事务数据的偏移
    long offset = getXidPosition(xid);
    //TODO 对文件从指定位置进行写入

}

long TransactionManager::getXidPosition(long xid) {
    // 根据头部长度 + 单个事务数据长度来计算
    return LEN_XID_HEADER_LENGTH + (xid - 1) * XID_FIELD_SIZE;
}

bool TransactionManager::checkStatus(long xid, unsigned char status) {
    long offset = getXidPosition(xid);
    // TODO 读取文件中的状态，这里写死为1
    unsigned char curStatus = '1';
    return curStatus == status;
}

void TransactionManager::incrXIDCounter() {
    xidCounter++;
    // TODO 写入到文件头当中
}