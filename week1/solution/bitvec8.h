#include "stdint.h"
#include "stdbool.h"

typedef uint8_t bitvec8;

// Convert, and possibly truncate, an unsigned int value
// into a bitvec8 value.
bitvec8 bitvec8_from_int(unsigned int x);

// Convert a bitvec8 value into an unsigned int value.
unsigned int bitvec8_to_int(bitvec8 x);

// prints a bitvec8 value.  
void bitvec8_print(bitvec8 v);
 
// Set bit at position i to 1
bitvec8 bitvec8_set(bitvec8 v, int i);

// Clear bit at position i to 0
bitvec8 bitvec8_clear(bitvec8 v, int i);

// Get bit at position i
bool bitvec8_get(bitvec8 v, int i);

// Check if all bits are set
bool bitvec8_all(bitvec8 v);

// Check if any bit is set
bool bitvec8_any(bitvec8 v);

// Check if no bits are set
bool bitvec8_none(bitvec8 v);

// Check if exactly one bit is set
bool bitvec8_one(bitvec8 v);
