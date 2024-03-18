#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

int number = 2000000;

void error(char *msg) {
    fprintf(stderr, "%s: %s", msg, strerror(errno));
    exit(1);
}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *decay_number(void *t) {
  pthread_mutex_lock(&lock);
  for (int i = 0; i < 100000; ++i) {
    number = number - 1;
  }
  pthread_mutex_unlock(&lock);
  return NULL;
}

int main() {
  void *result;
  int ste;
  pthread_t thread[20];

  for (int i = 0; i < (sizeof(thread)/sizeof(thread[0])); ++i) {
    ste = pthread_create(&thread[i], NULL, decay_number, NULL);
    if (ste == -1) {
      error("fail to create thread\n");
    }
  }
  for (int i = 0; i < (sizeof(thread) / sizeof(thread[0])); i++) {
    ste = pthread_join(thread[i], &result);
    if(ste == -1){
      error("스레드 종료 실패");
    }
  }
  printf("스레드 연산 실행 후 number: %d\n", number);




  return 0;
}