//
// Created by 40643 on 2022/2/20.
//

#ifndef DEMO_TRANSACTIONMANAGER_H
#define DEMO_TRANSACTIONMANAGER_H

#include "string"

class TransactionManager {
private:
    // XID文件头长度
    static int LEN_XID_HEADER_LENGTH;
    // 每个事务的占用长度
    static int XID_FIELD_SIZE;

    // 事务的三种状态
    static unsigned char FIELD_TRAN_ACTIVE;
    static unsigned char FIELD_TRAN_COMMITTED;
    static unsigned char FIELD_TRAN_ABORTED;

    // 超级事务，永远为committed状态
    static long SUPER_XID;

    static std::string XID_SUFFIX;

    long xidCounter;
public:
    long begin();
    void commit(long xid);
    void abort(long xid);
    bool isActive(long xid);
    bool isCommitted(long xid);
    bool isAborted(long xid);
    static TransactionManager* create(std::string path);
    static TransactionManager* open(std::string path);

    void updateXID(long xid, unsigned char active);

    long getXidPosition(long xid);

    bool checkStatus(long xid, unsigned char status);

    void incrXIDCounter();
};


#endif //DEMO_TRANSACTIONMANAGER_H
