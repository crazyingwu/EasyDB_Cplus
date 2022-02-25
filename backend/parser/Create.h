//
// Created by 40643 on 2022/2/25.
//

#ifndef EASYDB_CREATE_H
#define EASYDB_CREATE_H

#include "string"

/**
 * 创建表的语句抽象
 */
class Create {
public:
    std::string tableName;
    std::string* fieldName;
    std::string* fieldType;
    std::string* index;
}
#endif //EASYDB_CREATE_H
