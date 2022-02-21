//
// Created by 40643 on 2022/2/21.
//

#include "VersionManager.h"
#include "Entry.h"
#include "Visibility.h"

unsigned char* VersionManager::read(long xid, long uid) {
    // 先读取事务信息，xid为0的超级事务id已经提前放在了active当中
    mutex.lock();
    Transaction transaction = active.at(xid);
    mutex.unlock();
    // 从缓存中获取entry
    Entry entry = AbstractCache<Entry>::get(uid);
    // 判断可见性
    if (Visibility::isVisible(tm, transaction, entry)) {
        unsigned char* data = entry.data();
        entry.release();
        return data;
    }
    return nullptr;

}

/**
 * 调用dataManager来完成插入数据
 * @param xid
 * @param data
 * @return
 */
long VersionManager::insert(long xid, unsigned char *data) {
    mutex.lock();
    Transaction transaction = active.at(xid);
    mutex.unlock();
    unsigned char* raw = Entry::wrapEntryRaw(xid, data);
    dataManager.insert(xid, raw);
}

/**
 * delete方法涉及到了锁，所以相对会复杂点，步骤如下：
 * 1.获取到对应的entry
 * 2.检查可见性，不可见直接返回，拒绝删除
 * 3.获取锁，删除操作会涉及多个同时运行的事务竞争，所以需要控制
 * 4.获取锁出现问题会导致事务回滚
 * 5.检查版本跳跃，如果版本跳跃了，事务回滚
 * 6.最后把对应的entry的Xmax设置未当前的事务id，这样之后的事务都不可见该条记录
 * TODO 还是有点不明白回滚的具体操作
 * @param xid
 * @param uid
 * @return
 */
bool VersionManager::delete_(long xid, long uid) {
    Transaction t = active.at(xid);
    Entry entry = AbstractCache<Entry>::get(uid);
    if (!Visibility::isVisible(tm, t, entry)) {
        return false;
    }
    std::mutex* lock;
    lock = lockTable.add(xid, uid);
    if (lock != nullptr) {
        internAbort(xid, true);
    }

}

// TODO 没搞懂
void VersionManager::internAbort(long xid, bool autoAborted) {
    mutex.lock();
    Transaction t = active.at(xid);
    if (!autoAborted) {
        active.erase(xid);
    }
    mutex.unlock();
    if (t.autoAborted) {
        return;
    }
    // 释放资源
    lockTable.remove(xid);
    tm.abort(xid);
}

long VersionManager::begin(int level) {
    long xid = tm.begin();
    Transaction t = Transaction::newTransaction(xid, level, active);
    active.insert(std::pair<long, Transaction> (xid, t));
    return xid;
}

void VersionManager::commit(long xid) {
    Transaction t = active.at(xid);
    if (t.err.what() != nullptr) {
        throw t.err;
    }
    active.erase(xid);
    // 释放资源
    lockTable.remove(xid);
    tm.commit(xid);
}

void VersionManager::abort(long xid) {
    internAbort(xid, false);
}