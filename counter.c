#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

atomic_int ai;

void * increase() {
  for (int i = 0; i < 10000; ++i) {
    ai++;
  }
}

int main() {
  pthread_t thread[10];
  for (int i = 0; i < 10; ++i) {
    pthread_create(&thread[i], NULL, increase, NULL);
  }
  void* result;

  for (int i = 0; i < 10; ++i) {
    pthread_join(thread[i], &result);
  }
  printf("%d\n", ai);
}