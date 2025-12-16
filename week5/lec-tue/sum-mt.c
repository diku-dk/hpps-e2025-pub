#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_data {
  int chunk_begin;
  int chunk_end;
  long long int *sum;
  pthread_mutex_t *sum_mutex;
};

void* thread_func(void* data) {
  struct thread_data *thread_data =
    (struct thread_data*)data;
  long long int local_sum = 0;
  for (int i = thread_data->chunk_begin;
       i < thread_data->chunk_end;
       i++) {
    local_sum += i;
  }
  pthread_mutex_lock(thread_data->sum_mutex);
  *thread_data->sum += local_sum;
  pthread_mutex_unlock(thread_data->sum_mutex);

  return NULL;
}

int main(int argc, char** argv) {
  assert(argc == 3);
  int n = atoi(argv[1]);
  int nthread = atoi(argv[2]);

  int chunk_size = n / nthread;
  long long int sum = 0;
  pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER;

  struct thread_data *thread_data =
    malloc(nthread * sizeof(struct thread_data));

  pthread_t* threads =
    malloc(nthread * sizeof(pthread_t));

  for (int i = 0; i < nthread; i++) {
    thread_data[i].sum = &sum;
    thread_data[i].sum_mutex = &sum_mutex;
    thread_data[i].chunk_begin = i * chunk_size;
    thread_data[i].chunk_end = (i+1) * chunk_size;
    if (i == nthread-1) {
      thread_data[i].chunk_end = n;
    }
    pthread_create(&threads[i],
                   NULL,
                   &thread_func,
                   &thread_data[i]);
  }
  for (int i = 0; i < nthread; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("%lld\n", sum);
}
