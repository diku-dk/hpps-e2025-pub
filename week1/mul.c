#include "stdint.h"
#include "stdio.h"

typedef uint8_t byte;

byte mul(byte n){
  return n * 2;
}

int main(void) {
  printf("2 * 0xFF = %X\n", mul(255)); 
  return 0;
}


