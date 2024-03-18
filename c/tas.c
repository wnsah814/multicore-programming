#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
  atomic_bool flag;
} TASLock;

TASLock lock;

// void TASLock_init(TASLock *lock) {
//   atomic_init(&lock->flag, false);
// }

void TASLock_lock(TASLock *lock) {
  while (atomic_exchange(&lock->flag, true));
}

void TASLock_unlock(TASLock *lock) {
  atomic_store(&lock->flag, false);
}

void* thread_function(void *arg) {
  int id = *(int *)arg;
  int *counter = (int *)malloc(sizeof(int));
  *counter = 0;

  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, &start);

  const int EPOCH = 1000000;

  for (int j = 0; j < EPOCH; ++j) {
    TASLock_lock(&lock);
    (*counter)++;
    TASLock_unlock(&lock);
  }

  clock_gettime(CLOCK_REALTIME, &end);

  double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  printf("Thread#%d %lf seconds\n", id, elapsed);

  return counter;
}

int main() {
  pthread_t threads[10];
  int thread_ids[10];

  for (int i = 0; i < 10; ++i) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
  }

  int total = 0;
  int *retval;
  for (int i = 0; i < 10; ++i) {
    pthread_join(threads[i], (void **)&retval);
    total += *retval;
    free(retval);
  }

  printf("%d\n", total);

  return 0;
}
