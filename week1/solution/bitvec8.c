#include "bitvec8.h"

bitvec8 bitvec8_set(bitvec8 v, int i) {
  return v | (1 << i);
}

bitvec8 bitvec8_clear(bitvec8 v, int i) {
  return v & ~(1 << i);
}

bitvec8 bitvec8_toggle(bitvec8 v, int i) {
  return v ^ (1 << i);
}

bool bitvec8_get(bitvec8 v, int i) {
  return (v >> i) & 1;
}

int bitvec8_popcount(bitvec8 v) {
  v = (v & 0x55) + ((v >> 1) & 0x55);
  v = (v & 0x33) + ((v >> 2) & 0x33);
  v = (v & 0x0F) + ((v >> 4) & 0x0F);
  return v;
}

bool bitvec8_all(bitvec8 v) {
  return v == 0xFF;
}

bool bitvec8_any(bitvec8 v) {
  return v != 0;
}

bool bitvec8_none(bitvec8 v) {
  return v == 0;
}

bool bitvec8_one(bitvec8 v) {
  return !!v && (v & (-v)) == v;
}
