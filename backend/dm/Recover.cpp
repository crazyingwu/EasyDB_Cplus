//
// Created by 40643 on 2022/2/21.
//
#include "Recover.h"
#include "DataItem.h"
#include "PageX.h"
#include "map"
#include "vector"

void Recover::recover(TransactionManager tm, Logger logger, PageCache pageCache) {
    //重置log指针
    logger.rewind();
    int maxPgno = 0;
    for (;;) {
        unsigned char *log = logger.next();
        if (log == nullptr) break;
        int pgno;
        // 根据不同的日志类型，解析为相应类型的日志结构
        if (isInsertLog(log)) {
            InsertLogInfo li = parseInsertLog(log);
            pgno = li.pgno;
        } else {
            UpdateLogInfo li = parseUpdateLog(log);
            pgno = li.pgno;
        }

        // 记录最大的页号
        if (pgno > maxPgno) {
            maxPgno = pgno;
        }
    }
    if (maxPgno == 0) {
        maxPgno = 1;
    }

    //TODO 只保留最大页号的数据，后面的数据都会被丢失，这里为什么这么写
    pageCache.truncateByPgno(maxPgno);
    redoTranscations(tm, logger, pageCache);
    undoTranscations(tm, logger, pageCache);

}

bool Recover::isInsertLog(unsigned char *log) {
    return log[0] == LOG_TYPE_INSERT;
}

InsertLogInfo Recover::parseInsertLog(unsigned char *log) {
    return InsertLogInfo();
}

UpdateLogInfo Recover::parseUpdateLog(unsigned char *log) {
    return UpdateLogInfo();
}

void Recover::redoTranscations(TransactionManager tm, Logger logger, PageCache pc) {
    logger.rewind();
    for (;;) {
        unsigned char* log = logger.next();
        if (log == nullptr) break;
        if (isInsertLog(log)) {
            InsertLogInfo li = parseInsertLog(log);
            long xid = li.xid;
            if (!tm.isActive(xid)) {
                doInsertLog(pc, logger, REDO);
            }
        } else {
            UpdateLogInfo li = parseUpdateLog(log);
            long xid = li.xid;
            if (!tm.isActive(xid)) {
                doUpdateLog(pc, logger, REDO);
            }
        }
    }
}

void Recover::undoTranscations(TransactionManager tm, Logger logger, PageCache cache) {
    // undo需要倒序执行，所以用一个map存一下
    std::map<long, std::vector<unsigned char*>> logCache;
    logger.rewind();
    for (;;) {
        unsigned char* log = logger.next();
        if (log == nullptr) break;
        if (isInsertLog(log)) {
            InsertLogInfo li = parseInsertLog(log);
            long xid = li.xid;
            if (tm.isActive(xid)) {
                if (logCache.find(xid) == nullptr) {
                    logCache.insert(std::pair<long, std::vector<unsigned char*>>(xid,std::vector<unsigned char*>()));
                }
                logCache.at(xid).push_back(log);
            }
        } else {
            UpdateLogInfo li = parseUpdateLog(log);
            long xid = li.xid;
            if (tm.isActive(xid)) {
                if (logCache.find(xid) == nullptr) {
                    logCache.insert(std::pair<long, std::vector<unsigned char*>>(xid,std::vector<unsigned char*>()));
                }
                logCache.at(xid).push_back(log);
            }
        }

        // 对map中的log进行倒叙遍历
        std::map::iterator iterator = logCache.end();
        while(iterator > logCache.begin()) {
            unsigned char* log_ = iterator.first;
            if (isInsertLog(log_)) {
                doInsertLog(cache, log_, UNDO);
            } else {
                doUpdateLog(cache, log_, UNDO);
            }
        }
    }
}

/*
 * 不管是提交了的事务，还是回滚了的事务，只要事务里插入了数据，我们这里都会再把数据插入回去
 * TODO 问题1：为什么回滚了的还要插入回去？
 * 猜测：为了恢复的方便
 * 我们知道，判断一条记录是否可见的时候，需要用到修改这个记录的最后的事务id
 * 我们在回滚的时候，只是把事务id设为了回滚状态
 * 这样在判断可见性的时候，这条记录就是不可见的
 * 所以即使事务是回滚了，记录的数据还是应该存在的
 * 所以在恢复的时候还是需要插入回去，保证日志的完整
 * 如果是UNDO类型，我们需要把数据设为无效，再插入回去
 * */
void Recover::doInsertLog(PageCache cache, unsigned char* log, int flag) {

    InsertLogInfo li = parseInsertLog(log);
    Page pg;
    pg = cache.getPage(li.pgno);
    if (flag == UNDO) {
        DataItem::setDataItemRawInvalid(li.raw);
    }
    PageX::recoverInsert(pg, li.raw, li.offset);

}

void Recover::doUpdateLog(PageCache cache, unsigned char* log, int flag) {
    int pgno;
    short offset;
    unsigned char* raw;
    UpdateLogInfo li = parseUpdateLog(log);
    pgno = li.pgno;
    offset = li.offset;
    if (flag == REDO) {
        raw = li.newRaw;
    } else {
        raw = li.oldRaw;
    }
    Page pg = cache.getPage(pgno);
    PageX::recoverUpdate(pg, raw, offset);

}
