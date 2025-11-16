#include <stdio.h>
#include <assert.h>
#include "bitvec8.h"

void test_set(void) {
  uint8_t v = 0;
  v = bitvec8_set(v, 0);
  assert(v == 0x01);
  v = bitvec8_set(v, 3);
  assert(v == 0x09);
  v = bitvec8_set(v, 7);
  assert(v == 0x89);
  printf("test_set passed\n");
}

void test_clear(void) {
  uint8_t v = 0xFF;
  v = bitvec8_clear(v, 0);
  assert(v == 0xFE);
  v = bitvec8_clear(v, 3);
  assert(v == 0xF6);
  v = bitvec8_clear(v, 7);
  assert(v == 0x76);
  printf("test_clear passed\n");
}

void test_get(void) {
  uint8_t v = 0b10101010;
  assert(bitvec8_get(v, 0) == false);
  assert(bitvec8_get(v, 1) == true);
  assert(bitvec8_get(v, 2) == false);
  assert(bitvec8_get(v, 3) == true);
  assert(bitvec8_get(v, 7) == true);
  printf("test_get passed\n");
}

void test_all(void) {
  assert(bitvec8_all(0xFF) == true);
  assert(bitvec8_all(0xFE) == false);
  assert(bitvec8_all(0x00) == false);
  printf("test_all passed\n");
}

void test_any(void) {
  assert(bitvec8_any(0x00) == false);
  assert(bitvec8_any(0x01) == true);
  assert(bitvec8_any(0xFF) == true);
  printf("test_any passed\n");
}

void test_none(void) {
  assert(bitvec8_none(0x00) == true);
  assert(bitvec8_none(0x01) == false);
  assert(bitvec8_none(0xFF) == false);
  printf("test_none passed\n");
}

void test_one(void) {
  assert(bitvec8_one(0x00) == false);
  assert(bitvec8_one(0x01) == true);
  assert(bitvec8_one(0x02) == true);
  assert(bitvec8_one(0x04) == true);
  assert(bitvec8_one(0x08) == true);
  assert(bitvec8_one(0x10) == true);
  assert(bitvec8_one(0x20) == true);
  assert(bitvec8_one(0x40) == true);
  assert(bitvec8_one(0x80) == true);
  assert(bitvec8_one(0x03) == false);
  assert(bitvec8_one(0xFF) == false);
  assert(bitvec8_one(0xAA) == false);
  printf("test_one passed\n");
}

int main(void) {
  test_set();
  test_clear();
  test_get();
  test_all();
  test_any();
  test_none();
  test_one();

  printf("\nAll tests passed!\n");
  return 0;
}
