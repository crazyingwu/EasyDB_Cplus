//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_PAGEINFO_H
#define EASYDB_PAGEINFO_H

/**
 * 该类主要在建立page索引时使用
 */
class PageInfo{
public:
    int pgno;
    int freeSpace;

};

#endif //EASYDB_PAGEINFO_H
