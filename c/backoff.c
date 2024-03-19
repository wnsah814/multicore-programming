#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    atomic_bool flag;
} TTASLock;

TTASLock lock;

// void TTASLock_init(TTASLock *lock) {
//   atomic_init(&lock->flag, false);
// }

void TTASLock_lock(TTASLock *lock) {
  while (true) {
    while (atomic_load_explicit(&lock->flag, memory_order_relaxed));
    if (!atomic_exchange_explicit(&lock->flag, true, memory_order_acquire))
      return;
    struct timespec ts = {0, 100};
    nanosleep(&ts, NULL);
  }
}

void TTASLock_unlock(TTASLock *lock) {
  atomic_store_explicit(&lock->flag, false, memory_order_release);
}

void* thread_function(void *arg) {
  int id = *(int *)arg;
  int *counter = (int *)malloc(sizeof(int));
  *counter = 0;

  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, &start);

  const int EPOCH = 1000000;

  for (int j = 0; j < EPOCH; ++j) {
    TTASLock_lock(&lock);
    (*counter)++;
    TTASLock_unlock(&lock);
  }

  clock_gettime(CLOCK_REALTIME, &end);

  double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  printf("Thread#%d %lf seconds\n", id, elapsed);

  return counter;
}

int main() {
  srand(time(NULL));

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
