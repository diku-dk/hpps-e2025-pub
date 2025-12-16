#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

void* thread_func(void* data) {
  int* int_data = (int*)data;
  printf("hello from thread %d\n", *int_data);
  return NULL;
}

int main(int argc, char* argv[]) {
  assert(argc == 2);
  int nthreads = atoi(argv[1]);
  printf("creating %d threads\n", nthreads);
  pthread_t* new_thread = malloc(nthreads * sizeof(pthread_t));
  int* thread_input = malloc(nthreads * sizeof(int));
  for (int i = 0; i < nthreads; i++) {
    thread_input[i] = i;
    assert(pthread_create(&new_thread[i],
                          NULL,
                          &thread_func,
                          &thread_input[i])
           == 0);
  }
  for (int i = 0; i < nthreads; i++) {
    pthread_join(new_thread[i], NULL);
  }
  return 0;
}
