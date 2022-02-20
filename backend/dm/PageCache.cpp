//
// Created by 40643 on 2022/2/20.
//

#include "PageCache.h"

long pageOffset(int no);

int PageCache::newPage(unsigned char * initData) {
    int pageNo = ++pageNumber;
    Page page = Page(pageNo, initData, nullptr);
    flushPage(page);
    return pageNo;
}


Page PageCache::getForCache(long key){
    int pageNo = (int) key;
    long offset = pageOffset(pageNo);
    // 对文件加锁
    fileLock.lock();
    //TODO 读取文件，并包装为Page
    unsigned char* data = nullptr;
    return Page(pageNo, data, this);
}

void PageCache::releaseForCache(Page page){
    if (page.isDirty()) {
        this -> flushPage(page);
        page.setDirty(false);
    }
}

void PageCache::flushPage(Page pg) {
    // 拿到页号
    int pageNo = pg.getPageNumber();
    // 计算偏移
    long offset = this -> pageOffset(pageNo);
    fileLock.lock();
    // 写入pg.getData到文件中
    fileLock.unlock();
}


void PageCache::truncateByPgno(int maxPgno) {
    // 计算截断后的文件长度
    int size = this->pageOffset(maxPgno);
    // TODO 把文件长度直接设置为size大小，后面的数据就会被自动丢弃
    //设置pageNumber为截断后的大小
    pageNumber = maxPgno;
}


long PageCache::pageOffset(int no) {
    return 0;
}
