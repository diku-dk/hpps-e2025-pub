#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <assert.h>
#include <time.h>

double seconds() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t.tv_sec + (double)t.tv_nsec/1e9;
}

// input must point to n elements
// output must point to n elements
// returns number of non-negative elements
size_t filter(size_t n,
              int* input,
              int* output) {
  size_t o = 0;
  for (size_t i = 0; i < n; i++) {
    if (input[i] >= 0) {
      output[o] = input[i];
      o++;
    }
  }
  return o;
}

size_t filter_openmp(size_t n,
                     int* input,
                     int* output) {
  (void)n; (void)input; (void)output;

  int num_threads = omp_get_max_threads();

  int** filtered_chunks =
    malloc(num_threads * sizeof(int*));
  size_t* ks =
    malloc(num_threads * sizeof(size_t));

#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    size_t chunk_size = n / num_threads;
    size_t chunk_start = tid*chunk_size;

    if (tid == num_threads-1) {
      chunk_size = n - chunk_start;
    }

    int* tmp_output = malloc(chunk_size * sizeof(int));

    ks[tid] = filter(chunk_size,
                     &input[chunk_start],
                     tmp_output);

    filtered_chunks[tid] = tmp_output;
  }

  size_t o = 0;
  for (size_t tid = 0; tid < (size_t)num_threads; tid++) {
    size_t k = ks[tid];
    int* filtered_chunk = filtered_chunks[tid];
    for (size_t i = 0; i < k; i++) {
      output[o] = filtered_chunk[i];
      o++;
    }
    free(filtered_chunk);
  }

  return o;
}

size_t filter_openmp_better(size_t n,
                            int* input,
                            int* output) {
  (void)n; (void)input; (void)output;

  int num_threads = omp_get_max_threads();

  int** filtered_chunks =
    malloc(num_threads * sizeof(int*));
  size_t* ks =
    malloc(num_threads * sizeof(size_t));
  size_t chunk_size = n / num_threads;

#pragma omp parallel
  {
    int tid = omp_get_thread_num();
    size_t chunk_start = tid*chunk_size;

    if (tid == num_threads-1) {
      chunk_size = n - chunk_start;
    }

    int* tmp_output = malloc(chunk_size * sizeof(int));

    ks[tid] = filter(chunk_size,
                     &input[chunk_start],
                     tmp_output);

    filtered_chunks[tid] = tmp_output;
  }

  size_t* offsets = malloc(num_threads * sizeof(size_t));
  offsets[0] = 0;

  size_t o = ks[0];
  for (size_t tid = 1; tid < (size_t)num_threads; tid++) {
    offsets[tid] = offsets[tid-1] + ks[tid-1];
    o += ks[tid];
  }

#pragma omp parallel for
  for (size_t tid = 0; tid < (size_t)num_threads; tid++) {
    size_t k = ks[tid];
    int* filtered_chunk = filtered_chunks[tid];
    for (size_t i = 0; i < k; i++) {
      output[offsets[tid]+i] = filtered_chunk[i];
    }
  }

  for (size_t tid = 0; tid < (size_t)num_threads; tid++) {
    int* filtered_chunk = filtered_chunks[tid];
    free(filtered_chunk);
  }

  return o;
}

int main(int argc, char** argv) {
  assert(argc == 2);
  size_t n = atoi(argv[1]);

  int* input = malloc(n * sizeof(int));
  int* golden_output = malloc(n * sizeof(int));
  int* output = malloc(n * sizeof(int));

  for (size_t i = 0; i < n; i++) {
    input[i] = i % 10 - 5;
  }

  double bef, aft;

  bef = seconds();
  filter(n, input, golden_output);
  aft = seconds();

  printf("sequential runtime: %f seconds\n", aft-bef);

  bef = seconds();
  filter_openmp(n, input, output);
  aft = seconds();

  printf("openmp runtime: %f seconds\n", aft-bef);

  for (size_t i = 0; i < n; i++) {
    if (golden_output[i] != output[i]) {
      printf("mismatch at index %zd\n", i);
      return 1;
    }
  }

  bef = seconds();
  filter_openmp_better(n, input, output);
  aft = seconds();

  printf("openmp_better runtime: %f seconds\n", aft-bef);

  for (size_t i = 0; i < n; i++) {
    if (golden_output[i] != output[i]) {
      printf("mismatch at index %zd\n", i);
      return 1;
    }
  }
}
