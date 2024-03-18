#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <chrono>

class Lock {
  public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
};
