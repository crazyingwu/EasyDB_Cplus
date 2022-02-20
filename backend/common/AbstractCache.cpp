//
// Created by 40643 on 2022/2/20.
//

#include "AbstractCache.h"
#include "thread"

template<class T>
AbstractCache<T>::AbstractCache(int maxResource) {
    // 指定最大缓存数
    this->maxResource = maxResource;
}

template<class T>
T AbstractCache<T>::get(long key) {
    for(;;) {
        // 1、先加锁
        mutex.lock();
        try {
            if (getting.at(key)) {
                mutex.unlock();
                // 让线程休眠1ms，等待资源获取
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                continue;
            }
        } catch (std::out_of_range e) {
            continue;
        }
        if (cache.find(key)) {
            T obj = cache.at(key);
            references.insert(std::pair<long, int>(key, references.at(key) + 1));
            mutex.unlock();
            return obj;
        }

        if (maxResource > 0 && count == maxResource) {
            mutex.unlock();
            throw std::exception();
        }

        count++;
        getting.insert(std::pair<long, bool>(key, true));
        mutex.unlock();
        break;
    }
    // 下面是正式从数据源中获取数据
    T obj = getForCache(key);
    mutex.lock();
    getting.erase(key);
    cache.insert(std::pair<long, T>(key, obj));
    references.insert(std::pair<long, int>(key, 1));
    mutex.unlock();
    return obj;
}

template <class T>
void AbstractCache<T>::release(long key) {
    // 先加锁
    mutex.lock();
    int ref = references.at(key) - 1;
    // 将缓存写回
    if (ref == 0) {
        T obj = cache.at(key);
        releaseForCache(obj);
        cache.erase(key);
        count--;
    }
    mutex.unlock();
}

template <class T>
void AbstractCache<T>::close() {
    mutex.lock();
    typename std::map<long, T>::iterator iterator;
    iterator = cache.begin();
    while (iterator != cache.end()) {
        long key = iterator->first;
        release(key);
        references.erase(key);
        cache.erase(key);
    }
    mutex.unlock();
}