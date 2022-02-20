//
// Created by 40643 on 2022/2/20.
//

#ifndef EASYDB_PAGE_H
#define EASYDB_PAGE_H
#include "mutex"
#include "PageCache.h"
class Page {
private:
    bool dirty;
    unsigned char* data;
    int pageNumber;
    std::mutex mutex;
    PageCache pageCache;
public:
    Page(int pageNo, unsigned char* initData, PageCache* pageCache);
    void lock();
    void unlock();
    void release();
    void setDirty(bool dirty);
    bool isDirty();
    int getPageNumber();
    unsigned char* getData();
};


#endif //EASYDB_PAGE_H
