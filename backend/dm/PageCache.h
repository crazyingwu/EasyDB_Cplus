//
// Created by 40643 on 2022/2/20.
//

#ifndef EASYDB_PAGECACHE_H
#define EASYDB_PAGECACHE_H
#include "Page.h"
#include "../common/AbstractCache.h"

/*
 * pageCache作为page的管理类，不单单承担了缓存功能，而且也承担了管理功能
 * 新建page，获取page都是pageCache提供的功能
 */
class PageCache : public AbstractCache{
private:
    // TODO 这里应该是一个原子增长整数
    int pageNumber;
    std::mutex fileLock;
public:
    int newPage(unsigned char* initData);
    Page getPage();
    void close();
    void release(Page page);
    // 按照给定的页号，把后面的页全部截断丢弃，TODO 弄懂他有什么作用
    void truncateByPgno(int maxPgno);
    // TODO 这里啥意思，还没搞懂
    int getPageNumber();
    void flushPage(Page pg);

    long pageOffset(int no);
};


#endif //EASYDB_PAGECACHE_H
