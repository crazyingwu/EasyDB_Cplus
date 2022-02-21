//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_LOCKTABLE_H
#define EASYDB_LOCKTABLE_H

#include "map"
#include "vector"
#include "mutex"
class LockTable{
private:
    // 某个xid已经获取到的uid列表
    std::map<long, std::vector<long>> x2u;

    // 某个uid被某个xid持有
    std::map<long, long> u2x;

    // 正在等待uid的xid列表
    std::map<long, std::vector<long>> wait;

    // 正在等待的xid与对应的锁
    std::map<long, std::mutex> waitLock;

    // xid正在等待的uid
    std::map<long, long> waitU;

    // 一些操作需求的同步锁
    std::mutex mutex;

public:

    /**
     * 添加一个xid对uid的等待边
     * @param xid
     * @param uid
     * @return Null表示不用等待，返回一个mutex表示需要等待
     */
    std::mutex* add(long xid, long uid);

    /**
     * 释放掉一个事务xid的占用
     * @param xid
     */
    void remove(long xid);

    /**
     * 从等待队列中选择一个xid来占用uid
     * @param uid
     */
    void selectNewXID(long uid);

    /**
     * 使用深度优先搜索算法，判断图中是否有环，来判断是否有死锁
     * @return
     */
    bool hasDeadLock();

    /**
     * 深度搜索算法
     * @param xid
     * @return
     */
    bool dfs(long xid);

    /**
     * TODO 具体是干什么的还不知道
     * @param listMap
     * @param uid0
     * @param uid1
     */
    void removeFromList(std::map<long, std::vector<long>> listMap, long uid0, long uid1);

    /**
     * TODO
     * @param listMap
     * @param uid0
     * @param uid1
     */
    void putIntoList(std::map<long, std::vector<long>> listMap, long uid0, long uid1);

    /**
     * TODO
     * @param listMap
     * @param uid0
     * @param uid1
     */
    void isInList(std::map<long, std::vector<long>> listMap, long uid0, long uid1);

};

#endif //EASYDB_LOCKTABLE_H
