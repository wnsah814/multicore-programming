#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <chrono>

#define __BACK_OFF__x

using namespace std;

class Lock {
  public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
};

class TASLock : public Lock {
  // atomic<bool> state = ATOMIC_FLAG_INIT;
  atomic_flag flag;
  public:
  TASLock() : flag(false) {}

  void lock() {
    // auto start = chrono::high_resolution_clock::now();

    while (flag.test_and_set(memory_order_acquire)) {}
    // while (state.exchange(true)) {}

    // auto end = chrono::high_resolution_clock::now();
    // chrono::duration<double> duration = end - start;
    // cout << "TAS " << duration.count() << " seconds\n";
  }

  void unlock() {
    flag.clear(memory_order_release);
  }
};

class TTASLock : public Lock {
  atomic<bool> flag;

  public:
  TTASLock() : flag(false) {}

  void lock() {
    while (true) {
      while (flag.load(memory_order_relaxed)) {};
      if (!flag.exchange(true, memory_order_acquire)) return;

      #ifdef __BACK_OFF__
      this_thread::sleep_for(chrono::nanoseconds(100)); // backoff
      #endif
    }
  }

  void unlock() {
    flag.store(false, memory_order_release);
  }
};

class Backoff {
  const int minDelay, maxDelay;
  int limit;
  
};

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

TTASLock lock;
int counter = 0;

int main() {
  vector<thread> threads;
  const int SIZE = 10;
  const int EPOCH = 1'000'000;

  for (int i = 0; i < SIZE; ++i) {
    threads.emplace_back([](int id) {
      auto start = chrono::high_resolution_clock::now();
    
      for (int j = 0; j < EPOCH; ++j) {
        lock.lock();
        counter++;
        // cout << id << endl;
        lock.unlock();
      }

      auto end = chrono::high_resolution_clock::now();
      chrono::duration<double> duration = end - start;
      cout << "Thread#" << id << " " << duration.count() << " seconds\n";
    }, i);
  }

  for (int i = 0; i < SIZE; ++i) {
    threads[i].join();
  }

  cout << counter << endl;

  return 0;
}