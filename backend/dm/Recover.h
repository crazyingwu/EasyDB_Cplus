//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_RECOVER_H
#define EASYDB_RECOVER_H

#include "../tm/TransactionManager.h"
#include "PageCache.h"
#include "Logger.h"

#define LOG_TYPE_INSERT 0
#define LOG_TYPE_UPDATE 1
#define REDO 0
#define UNDO 1

struct InsertLogInfo{
    long xid;
    int pgno;
    short offset;
    unsigned char* raw;
};

struct UpdateLogInfo{
    long xid;
    int pgno;
    short offset;
    unsigned char* oldRaw;
    unsigned char* newRaw;
};

/**
 * 日志恢复的理论
 * 1. 保证正在进行的事务，不会读取其他未提交事务的数据，即防止脏读，脏读会带来级联回滚等问题
 * 2. 保证不会脏写，即正在进行的事务，不会修改其他任何未提交事务修改或产生的数据
 * 3. 在前两个规则的保证下，我们根据操作的日志，进行相应的恢复：
 *  - 如果是insert，就删除掉对应的数据
 *  - 如果是update，就恢复到旧值，update日志中保存有旧值
 */
class Recover{
public:

    /* 根据日志进行数据恢复
     * tm: 检查对应的事务的状态，是活跃还是已提交还是回滚
     * logger: 读取日志
     * pageCache: 根据日志重写页面数据
     * */
    static void recover(TransactionManager tm, Logger logger, PageCache pageCache);


    static bool isInsertLog(unsigned char *log);

    static InsertLogInfo parseInsertLog(unsigned char *log);

    static UpdateLogInfo parseUpdateLog(unsigned char *log);

    static void redoTranscations(TransactionManager manager, Logger logger, PageCache cache);

    static void undoTranscations(TransactionManager manager, Logger logger, PageCache cache);

    static void doInsertLog(PageCache cache, unsigned char* log, int i);

    static void doUpdateLog(PageCache cache, unsigned char* log, int i);
};

#endif //EASYDB_RECOVER_H
