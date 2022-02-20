cmake_minimum_required(VERSION 3.21)
project(EasyDB)

set(CMAKE_CXX_STANDARD 14)

add_executable(EasyDB main.cpp backend/tm/TransactionManager.h backend/tm/TransactionManager.cpp backend/common/AbstractCache.cpp backend/common/AbstractCache.h)
