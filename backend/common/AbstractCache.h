//
// Created by 40643 on 2022/2/20.
//

#ifndef EASYDB_ABSTRACTCACHE_H
#define EASYDB_ABSTRACTCACHE_H
#include "map"
#include "string"
#include "mutex"
template <class T>
class AbstractCache {
private:
    // 实际缓存的map
    std::map<long, T> cache;
    // 引用计数的map
    std::map<long, int> references;
    // 正在获取资源的线程
    std::map<long, bool> getting;
    // 缓存最大缓存数
    int maxResource;
    // 当前缓存数量
    int count;
    std::mutex mutex;

public:
    AbstractCache(int maxResource);

protected:
    T get(long key);
    void release(long key);
    // 关闭所有缓存，回写缓存
    void close();

    virtual T getForCache(long key);
    virtual void releaseForCache(T obj);
};


#endif //EASYDB_ABSTRACTCACHE_H
