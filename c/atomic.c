#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

atomic_int counter = 0;

void* thread_function(void *arg) {
  int id = *(int *)arg;

  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, &start);

  const int EPOCH = 1000000;

  for (int j = 0; j < EPOCH; ++j) {
    counter++;
  }

  clock_gettime(CLOCK_REALTIME, &end);

  double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  printf("Thread#%d %lf seconds\n", id, elapsed);

}

int main() {
  pthread_t threads[10];
  int thread_ids[10];

  for (int i = 0; i < 10; ++i) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
  }

  for (int i = 0; i < 10; ++i) {
    pthread_join(threads[i], NULL);
  }

  printf("%d\n", counter);

  return 0;
}