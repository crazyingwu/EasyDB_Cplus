//
// Created by 40643 on 2022/2/21.
//

#ifndef EASYDB_LOGGER_H
#define EASYDB_LOGGER_H

#include "string"
/**
 * 首先明白整个日志文件的定义
 * 日志文件标准格式为：
 * [XChecksum] [Log1] [Log2] ... [LogN] [BadTail]
 * XChecksum 为后续所有日志计算的Checksum，int类型
 * badTail是还没有来得及写入完整的日志，即一些写了一部分的日志
 * 这部分日志需要丢弃
 *
 * 每条正确日志的格式为：
 * [Size] [Checksum] [Data]
 * Size 4字节int 标识Data长度
 * Checksum 4字节int
 *
 * 日志的具体内容如下：
 * insertLog
 * [LogType] [XID] [Pgno] [Offset] [Raw]
 *
 * updateLog
 * [LogType] [XID] [UID] [OldRwa] [NewRaw]
 */
class Logger{
private:

public:
    /*
     * 创建日志文件
     * 头部XChecksum位置写0
     * */
    Logger create(std::string path);

    /*
     * 打开日志文件
     * 调用init()进行日志检查&恢复
     * */
    Logger open(std::string path);

    /*
     * 1. 读取出日志文件中XChecksum
     * 2. 调用checkAndRemoveTail()来检查日志文件是否完整，丢失最后写入不完整的日志
     * */
    void init();

    /*
     * 检查并移除bad tail
     * 主要步骤如下：
     * 1. 通过迭代器不断读取日志，如果读取为NULL，说明读取完了或者读取到badTail了
     * 2. 计算单条日志的校验，然后累加起来得到一个checksum
     * 3. 与XChecksum作比较，比较两者是否相等，如果不相等，直接停机，因为出现了错误日志文件
     * 4. 最后根据读取的position截断文件，只保留正确读取到了的日志文件
     * */
    void checkAndRemoveTail();

    /*
     * 1. 调用wrapLog得到日志数据
     * 2. 写入log数据到日志文件
     * 3. 调用updateXChecksum()更新日志文件头部的XChecksum
     * */
    void log(unsigned char* data);

    /*
     * 把数据包装为log数据，主要为：
     * 1. 计算校验和
     * 2. 计算数据长度
     * 3. 合并数据，得到[checksum][length][data]的结构
     * */
    unsigned char* wrapLog(unsigned char* data);

    /*
     * 1. 计算XChecksum的累计
     * 2. 写入到文件
     * */
    void updateXChecksum(unsigned char* log);

    /*
     * 截断文件到x的位置
     * */
    void truncate(long x);


    unsigned char* next();

    /*
     * 根据position的位置读取下一条日志
     * 1.读取日志的长度，并检查position+size是否超出的文件长度
     * 2.根据长度读取出日志的checksum+data
     * 3.计算data的checksum，并和读取出的checksum进行比较
     * 4.如果checksum没校验通过，返回null
     * 5.更新position += log.length
     * */
    unsigned char* internNext();

    /*
     * 重新定位position到第一条日志所在位置
     * 即4
     * */
    void rewind();

    /*
     * 关闭文件
     * */
    void close();
};

#endif //EASYDB_LOGGER_H
