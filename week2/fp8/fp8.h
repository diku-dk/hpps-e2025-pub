#include <stdint.h>

// FP8 E4M3 format: 1 sign bit, 4 exponent bits, 3 significans  bits
// - Exponent bias: 7
// 
// Convert FP8 E4M3 to float
float fp8_e4m3_to_float(uint8_t fp8);

// Convert float to FP8 E4M3
uint8_t float_to_fp8_e4m3(float f);
