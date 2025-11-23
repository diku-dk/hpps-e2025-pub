#include "fp8.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// helper functions
static inline uint32_t float_to_bits(float f) {
  uint32_t bits;
  memcpy(&bits, &f, sizeof(f));
  return bits;
}

static inline float bits_to_float(uint32_t bits) {
  float f;
  memcpy(&f, &bits, sizeof(f));
  return f;
}

// Convert FP8 E4M3 to float
float fp8_e4m3_to_float(uint8_t fp8) {
  uint32_t sign = (fp8 >> 7) & 0x1; // 1 bit
  uint32_t exp = (fp8 >> 3) & 0xF;  // 4 bits
  uint32_t significand = fp8 & 0x7; // 3 bits

  // special cases
  if (exp == 0xF) { 
    if (significand == 0) {
      // Infinity
      uint32_t f32_bits = (sign << 31) | (0xFF << 23);
      return bits_to_float(f32_bits);
    } else {
      // NaN
      uint32_t f32_bits = (sign << 31) | (0xFF << 23) | (significand << 20);
      return bits_to_float(f32_bits);
    }
  }

  if (exp == 0) {
    if (significand == 0) {
      // Zero
      uint32_t f32_bits = sign << 31;
      return bits_to_float(f32_bits);
    } else {
      // Subnormal number: e = 1 - b (b is 7 for fp8 and 127 for fp32) 
      int shift = 0; // how fp8 significand leading 0s impact fp32 exponent 
      uint32_t m = significand;
      while ((m & 0x8) == 0) {  // Find leading 1 in 3-bit significand
        m = m << 1; // arithmetic shift
        shift++;
      }
      m &= 0x7;  // Remove the leading 1 (it is part of the fp32 exponent) 
      // f32 exponent: (1 - 7) - shift + 127 = 121 - shift
      uint32_t f32_exp = 121 - shift;
      uint32_t f32_significand = m << 20;  // Shift to f32 significand position
      uint32_t f32_bits = (sign << 31) | (f32_exp << 23) | f32_significand;
      return bits_to_float(f32_bits);
    }
  }

  // Normal number
  // fp32 exponent:  E4M3_exp - 7 + 127 = E4M3_exp + 120
  uint32_t f32_exp = exp + 120;
  uint32_t f32_significand = significand << 20;  // Shift 3-bit significand to f32 23-bit position
  uint32_t f32_bits = (sign << 31) | (f32_exp << 23) | f32_significand;
  return bits_to_float(f32_bits);
}


// Convert float to FP8 E4M3
uint8_t float_to_fp8_e4m3(float f) {
  uint32_t f32_bits = float_to_bits(f);

  // Extract components
  uint32_t sign = (f32_bits >> 31) & 0x1;
  uint32_t exp = (f32_bits >> 23) & 0xFF;
  uint32_t significand = f32_bits & 0x7FFFFF;

  // Handle special cases
  if (exp == 0xFF) {  // Infinity or NaN
    if (significand == 0) {
      // Infinity
      return (sign << 7) | (0xF << 3);
    } else {
      // NaN
      return (sign << 7) | (0xF << 3) | 0x1;
    }
  }

  if (exp == 0 && significand == 0) {
    // Zero
    return sign << 7;
  }

  // Convert exponent: f32_exp - 127 + 7 = f32_exp - 120
  int32_t fp8_exp = (int32_t)exp - 120;

  // Handle overflow (too large)
  if (fp8_exp >= 0xF) {
    // Return infinity
    return (sign << 7) | (0xF << 3);
  }

  // Handle underflow (too small)
  if (fp8_exp <= 0) {
    if (fp8_exp < -3) {
      // Too small, return zero
      return sign << 7;
    }
    // Subnormal - shift significand right
    uint32_t fp8_significand = (0x8 | (significand >> 20)) >> (1 - fp8_exp);
    if (fp8_significand > 7) fp8_significand = 7;
    return (sign << 7) | (fp8_significand & 0x7);
  }

  // Normal number - round significand to 3 bits
  uint32_t fp8_significand = (significand >> 20) & 0x7;

  // Simple rounding: round to nearest
  if ((significand >> 19) & 0x1) {  // Check rounding bit
    fp8_significand++;
    if (fp8_significand > 7) {  // Overflow in significand
      fp8_significand = 0;
      fp8_exp++;
      if (fp8_exp >= 15) {
        // Overflow to infinity
        return (sign << 7) | (0xF << 3);
      }
    }
  }

  return (sign << 7) | ((fp8_exp & 0xF) << 3) | (fp8_significand & 0x7);
}

