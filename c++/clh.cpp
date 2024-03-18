#include "lock.h"
using namespace std;

class CLHLock : public Lock {
private:
  class QNode {
    public:
    atomic<bool> locked = {false};
  };

  std::atomic<QNode*> tail;
  thread_local QNode* myNode;
  thread_local QNode* myPred;

public:
  CLHLock() : tail(new QNode()), myNode(new QNode()), myPred(nullptr) {}

  void lock() {
    QNode* qnode = myNode; // use my node
    qnode->locked.store(true); // announce start
    // Make me the new tail, and find my predecessor
    QNode* pred = tail.exchange(qnode);
    myPred = pred; // remember predecessor
    while (pred->locked.load()) {} // spin
  }

  void unlock() {
    QNode* qnode = myNode; // use my node
    qnode->locked.store(false); // announce finish
    myNode = myPred; // reuse predecessor
  }
};