//
// Created by 40643 on 2022/2/25.
//

#ifndef EASYDB_TABLE_H
#define EASYDB_TABLE_H
#include "string"
#include "vector"
#include "Field.h"
#include "TableManager.h"
#include "../parser/Create.h"

struct CalWhereRes {
    long l0, r0, l1, r1;
    bool single;
};

/**
 * Table 维护了表结构
 * 二进制结构如下：
 * [TableName][NextTable]
 * [Field1Uid][Field2Uid]...[FieldNUid]
 */
class Table {
    TableManager tbm;
    long uid;
    std::string name;
    unsigned char status;
    long nextUid;
    std::vector<Field> fields;
    /**
     * 加载一张表
     * @param tbm
     * @param uid
     * @return
     */
    static Table loadTable(TableManager tbm, long uid);

    /**
     * 创建一张表
     * @param tbm
     * @param nextUid
     * @param xid
     * @param create
     * @return
     */
    static Table createTable(TableManager tbm, long nextUid, long xid, Create create);

    /**
     * 将数据解析为表的结构
     * @param raw
     * @return
     */
    Table parseSelf(unsigned char* raw);

    /**
     * 将表结构本身持久化到数据源
     * @param xid
     * @return
     */
    Table persistSelf(long xid);

    /**
     * 删除表中的数据
     * @param xid
     * @return
     */
    int delete_(long xid, Delete delete_);

    /**
     * 更新表中的数据
     * @param xid
     * @param update
     * @return
     */
    int update(long xid, Update update);

    /**
     * 读出表中的数据
     * @param xid
     * @param read
     * @return
     */
    std::string read(long xid, Select read);

    /**
     * 向表中插入数据
     * @param xid
     * @param insert
     */
    void insert(long xid, Insert insert);

    /**
     * 根据where条件，筛选出满足的uid
     * @param where
     * @return
     */
    std::vector<long> parseWhere(Where where);

    /**
     * TODO 计算where表达式
     * @param fd
     * @param where
     * @return
     */
    CalWhereRes calWhere(Field fd, Where where);
};

#endif //EASYDB_TABLE_H
