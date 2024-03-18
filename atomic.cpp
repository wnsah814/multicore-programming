#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> counter(0);

void incrementCounter() {
  for (int i = 0; i < 1000000; ++i) {
    counter.fetch_add(1, std::memory_order_relaxed);
  }
}

void decrementCounter() {
  for (int i = 0; i < 1000000; ++i) {
    counter.fetch_sub(1, std::memory_order_relaxed);
  }
}

int main() {
  std::thread t1(incrementCounter);
  std::thread t2(decrementCounter);

  t1.join();
  t2.join();

  std::cout << "Counter value: " << counter << std::endl;

  return 0;
}
