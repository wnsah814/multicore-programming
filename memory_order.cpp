#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
using namespace std;

// using std::memory_order_relaxed;
// using std::memory_order_release;
// using std::memory_order_acquire;
// using std::cout;
// using std::endl;
// using std::vector;

atomic<int> data[3];
atomic<bool> is_ready;

void producer() {
  data[0].store(1, memory_order_relaxed);
  data[1].store(2, memory_order_relaxed);
  data[2].store(3, memory_order_relaxed);
  is_ready.store(true, memory_order_release);
}

void consumer() {
  while (!is_ready.load(memory_order_acquire)) {

  }
  cout << "data[0]: " << data[0].load(memory_order_relaxed) << endl;
  cout << "data[1]: " << data[1].load(memory_order_relaxed) << endl;
  cout << "data[2]: " << data[2].load(memory_order_relaxed) << endl;
}

int main() {
  vector<thread> threads;

  threads.push_back(thread(producer));
  threads.push_back(thread(consumer));

  for (int i = 0; i < 2; ++i) {
    threads[i].join();
  }

}