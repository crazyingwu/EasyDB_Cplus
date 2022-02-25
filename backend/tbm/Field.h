//
// Created by 40643 on 2022/2/25.
//

#ifndef EASYDB_FIELD_H
#define EASYDB_FIELD_H

#include "string"
#include "vector"
#include "../im/BPlusTree.h"
#include "FieldCalRes.h"
#include "../parser/SingleExpression.h"

struct ParseValueRes {
    void* v;
    int shift;
};

/**
 * Field 表示字段信息
 * 二进制格式为：
 * [FieldName][TypeName][IndexUid]
 * 如果field无索引，IndexUid为0
 */
class Field {
private:
    long uid;
//    Table table;
    std::string fieldName;
    std::string fieldType;
    long index;
    BPlusTree bt;
public:
    /**
     * 从文件中加载一个字段信息
     * @param tb
     * @param uid
     * @return
     */
    static Field loadField(Table tb, long uid);

    /**
     * 将byte数据解析为Field结构
     * @param raw
     * @return
     */
    Field parseSelf(unsigned char* raw);

    /**
     * 创建一个字段信息
     * @param tb
     * @param xid
     * @param fieldName
     * @param fieldType
     * @param indexed
     * @return
     */
    static Field createField(Table tb, long xid, std::string fieldName,
                             std::string fieldType, bool indexed);

    /**
     * 持久化本身数据到数据源，insert数据
     * @param xid
     */
    void persistSelf(long xid);


    /**
     * 检查当前字段的类型是否是给定类型
     * @param fieldType
     */
    static void typeCheck(std::string fieldType);

    /**
     * 检查当前字段是否是索引
     * @return
     */
    bool isIndexed();

    /**
     * TODO 这里的插入是什么意思？
     * 插入到B+树当中
     * @param key
     * @param uid
     */
    void insert(void* key, long uid);

    /**
     * 在B+树中进行搜索
     * @param left
     * @param right
     * @return
     */
    std::vector<long> search(long left, long right);

    /**
     * 给定一个字段的真实数据，解析出他的值和偏移
     * @param raw
     * @return
     */
    ParseValueRes parserValue(unsigned char* raw);

    /**
     * TODO 没搞懂，貌似是计算where表达式的时候使用
     * @param exp
     * @return
     */
    FieldCalRes calExp(SingleExpression exp);
};

#endif //EASYDB_FIELD_H
