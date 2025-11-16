#include "stdio.h"
#include "bitvec8.h"

bitvec8 bitvec8_from_int(unsigned int x) {
  return 0xFF & x;
}

unsigned int bitvec8_to_int(bitvec8 v) {
  return v;
}

void bitvec8_print(bitvec8 v) {
  putchar('0' + ((v>>7) & 1));
  putchar('0' + ((v>>6) & 1));
  putchar('0' + ((v>>5) & 1));
  putchar('0' + ((v>>4) & 1));
  putchar('0' + ((v>>3) & 1));
  putchar('0' + ((v>>2) & 1));
  putchar('0' + ((v>>1) & 1));
  putchar('0' + (v & 1));
}
