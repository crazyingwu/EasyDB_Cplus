//
// Created by 40643 on 2022/2/24.
//

#ifndef EASYDB_BPLUSTREE_H
#define EASYDB_BPLUSTREE_H

#include "vector"
#include "../dm/DataManager.h"

struct InsertRes {
    long newNode, newKey;
};

/**
 * 向上层提供两个能力：
 * 1.插入索引
 * 2.搜索节点
 * 为什么不提供删除索引的能力？
 * 删除通过拿到节点后，设置XMAX来实现
 */
class BPlusTree{
private:

    DataManager dm;
    long bootUid;
    DataItem bootDataItem;
    std::mutex bootLock;
public:
    /**
     * 新建一个B+树索引
     * 1.新建一个根节点
     * 2.插入数据到dm中
     * 3.TODO 这里又把rootUid插入到了dm是什么意思
     * @param dm
     * @return
     */
    static long create(DataManager dm);

    /**
     * 根据bootUid加载一个B+树索引
     * @param bootUid
     * @param dm
     * @return
     */
    static BPlusTree load(long bootUid, DataManager dm);

    /**
     * TODO 获取rootUid
     * @return
     */
    long rootUid();

    /**
     * 更新根节点
     * @param left
     * @param right
     * @param rightKey
     */
    void updateRootUid(long left, long right, long rightKey);

    /**
     * 搜索一个叶子节点
     * @param nodeUid
     * @param key
     * @return
     */
    long searchLeaf(long nodeUid, long key);

    /**
     * TODO 搜索下一个节点
     * @param nodeUid
     * @param key
     * @return
     */
    long searchNext(long nodeUid, long key);

    /**
     * 范围搜索
     * @param key
     * @return
     */
    std::vector<long> search(long key);

    /**
     * 范围搜索
     * @param leftKey
     * @param rightKey
     * @return
     */
    std::vector<long> searchRange(long leftKey, long rightKey);

    /**
     * 插入节点
     * @param key
     * @param uid
     */
    void insert(long key, long uid);

    /**
     * TODO 插入节点，带rootUid参数
     * @param nodeUid
     * @param uid
     * @param key
     * @return
     */
    InsertRes insert(long nodeUid, long uid, long key);

    /**
     * 插入节点，并自动分裂
     * @param nodeUid
     * @param uid
     * @param key
     * @return
     */
    InsertRes insertAndSplit(long nodeUid, long uid, long key);

    /**
     * TODO 释放B+树
     * 调用bootDateItem.release()
     */
    void close();

};
#endif //EASYDB_BPLUSTREE_H
