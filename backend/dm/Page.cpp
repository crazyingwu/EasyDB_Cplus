//
// Created by 40643 on 2022/2/20.
//

#include "Page.h"

void Page::lock() {
    mutex.lock();
}

void Page::unlock() {
    mutex.unlock();
}

void Page::release() {
    pageCache.release(this);
}

Page::Page(int pageNo, unsigned char *initData, PageCache *pageCache) {

}
