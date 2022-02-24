//
// Created by 40643 on 2022/2/24.
//

#ifndef EASYDB_NODE_H
#define EASYDB_NODE_H

#include "vector"
#include "../dm/DataItem.h"
#include "BPlusTree.h"

struct SearchNextRes {
    long uid;
    long siblingUid;
};

struct LeafSearchRangeRes {
    std::vector<long> uids;
    long siblingUid;
};

struct InsertAndSplitRes {
    long siblingUid, newSon, newKey;
};

struct InsertRes {
    long newNode, newKey;
};

struct SplitRes {
    long newSon, newKey;
};
/**
 * Node结构如下：
 * [LeafFlag][KeyNumber][SiblingUid]
 * [Son0][Key0][Son1][Key1]...[SonN][KeyN]
 * 最后一个keyN始终为MAX_VALUE
 * SiblingUid是兄弟节点在dm中的uid
 */
class Node {
private:
    // 是否是叶子节点的数据偏移
    static const int IS_LEAF_OFFSET = 0;
    // keyNumber数据偏移
    static const int NO_KEYS_OFFSET = IS_LEAF_OFFSET + 1;
    // SiblingUid数据的偏移
    static const int SIBLING_OFFSET = NO_KEYS_OFFSET + 2;
    // Node头部数据的大小
    static const int NODE_HEADER_SIZE = SIBLING_OFFSET + 8;
    // TODO what fuck?
    static const int BALANCE_NUMBER = 32;
    static const int NODE_SIZE = NODE_HEADER_SIZE + (2 * 8) * (BALANCE_NUMBER * 2 + 2);

    BPlusTree tree;
    DataItem dataItem;
    unsigned char *raw;
    long uid;

public:

    /**
     * 设置一个Node数据是否是叶子节点
     * @param raw
     * @param isLeaf
     */
    static void setRawIsLeaf(unsigned char* raw, bool isLeaf);

    /**
     * 判断Node是否是叶子节点
     * @param raw
     * @return
     */
    static bool getRawIfLeaf(unsigned char* raw);

    /**
     * 设置keyNumber
     * @param raw
     * @param noKeys
     */
    static void setRawNoKeys(unsigned char* raw, int noKeys);

    /**
     * 获取keyNumber
     * @param raw
     * @return
     */
    static int getRawNoKeys(unsigned char* raw);

    /**
     * 获取兄弟节点uid
     * @param raw
     * @param sibling
     */
    static void setRawSibling(unsigned char* raw, long sibling);

    static long getRawSibling(unsigned char* raw);

    /**
     * 设置第kth个子节点
     * @param raw
     * @param uid
     * @param kth
     */
    static void setRawKthSon(unsigned char* raw, long uid, int kth);

    static long getRawKthSon(unsigned char* raw, int kth);

    /**
     * 设置第kth个key
     * @param raw
     * @param key
     * @param kth
     */
    static void setRawKthKey(unsigned char* raw, long key, int kth);
    static long getRawKthKey(unsigned char* raw, int kth);


    /**
     * TODO 复制节点数据，具体用途未知
     * @param from
     * @param to
     * @param kth
     */
    static void copyRawFromKth(unsigned char* from, unsigned char* to, int kth);

    /**
     * TODO 数据移位，目的未知
     * @param raw
     * @param kth
     */
    static void shiftRawKth(unsigned char* raw, int kth);

    /**
     * 新建一个根节点
     * @param left
     * @param right
     * @param key
     * @return
     */
    static unsigned char* newRootRaw(long left, long right, long key);

    /**
     * 新建一个空的根节点
     * @return
     */
    static unsigned char* newNilRootRaw();

    /**
     * 从dataItem中根据uid加载一个node
     * @param bTree
     * @param uid
     * @return
     */
    static Node loadNode(BPlusTree bTree, long uid);

    /**
     * 释放当前数据dataItem
     */
    void release();

    /**
     * 是否是叶子节点
     * @return
     */
    bool isLeaf();

    /**
     * 寻找对应key的UID，如果找不到，则返回兄弟节点的UID
     * @param key
     * @return
     */
    SearchNextRes searchNext(long key);

    /**
     * TODO 还不太明白，在当前节点进行范围查找，范围为[leftKey, rightKey]
     * 如果rightKey大于等于该节点的最大key，则还同时返回兄弟节点的UID，方便继续搜索下一节点
     * @param leftKey
     * @param rightKey
     * @return
     */
    LeafSearchRangeRes leafSearchRange(long leftKey, long rightKey);

    /**
     * 插入节点，如果需要分割就会分割为两个节点
     * @param uid
     * @param key
     * @return
     */
    InsertAndSplitRes insertAndSplit(long uid, long key);

    /**
     * 插入节点
     * @param uid
     * @param key
     * @return
     */
    bool insert(long uid, long key);

    /**
     * 插入节点
     * @param nodeUid
     * @param uid
     * @param key
     * @return
     */
    InsertRes insertAndSplit(long nodeUid, long uid, long key);

    /**
     * 判断是否需要分割
     * @return
     */
    bool needSplit();

    /**
     * 分割节点
     * @return
     */
    SplitRes split();


};

#endif //EASYDB_NODE_H
