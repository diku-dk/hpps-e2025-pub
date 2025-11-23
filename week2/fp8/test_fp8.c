#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "fp8.h"

// Helper function to print FP8 bits
void print_fp8_bits(uint8_t fp8) {
    printf("0x%02x (", fp8);
    for (int i = 7; i >= 0; i--) {
        printf("%d", (fp8 >> i) & 1);
        if (i == 7 || i == 3) printf(" ");
    }
    printf(")");
}

// Helper function to compare floats (handles NaN and infinity)
int float_equal(float a, float b) {
    if (isnan(a) && isnan(b)) return 1;
    if (isinf(a) && isinf(b) && signbit(a) == signbit(b)) return 1;
    if (a == b) return 1;
    return 0;
}

// Test conversion from fp8 to float
void test_fp8_to_float(const char* name, uint8_t fp8, float expected) {
    float result = fp8_e4m3_to_float(fp8);
    int passed = float_equal(result, expected);

    printf("%-30s: ", name);
    print_fp8_bits(fp8);
    printf(" -> %g (expected %g) %s\n", result, expected, passed ? "PASS" : "FAIL");
}

// Test conversion from float to fp8
void test_float_to_fp8(const char* name, float f, uint8_t expected) {
    uint8_t result = float_to_fp8_e4m3(f);
    int passed = (result == expected);

    printf("%-30s: %g -> ", name, f);
    print_fp8_bits(result);
    printf(" (expected ");
    print_fp8_bits(expected);
    printf(") %s\n", passed ? "PASS" : "FAIL");
}

// Test round-trip conversion
void test_roundtrip(const char* name, uint8_t fp8) {
    float f = fp8_e4m3_to_float(fp8);
    uint8_t fp8_back = float_to_fp8_e4m3(f);
    int passed = (fp8 == fp8_back);

    printf("%-30s: ", name);
    print_fp8_bits(fp8);
    printf(" -> %g -> ", f);
    print_fp8_bits(fp8_back);
    printf(" %s\n", passed ? "PASS" : "FAIL");
}

int main() {
    printf("=== FP8 E4M3 Library Tests ===\n\n");

    // FP8 E4M3 format: 1 sign bit, 4 exponent bits, 3 significand bits
    // Exponent bias: 7

    printf("--- Testing FP8 to Float Conversion ---\n\n");

    // Zero
    test_fp8_to_float("Positive zero", 0x00, 0.0f);
    test_fp8_to_float("Negative zero", 0x80, -0.0f);

    // Infinity
    test_fp8_to_float("Positive infinity", 0x78, INFINITY);
    test_fp8_to_float("Negative infinity", 0xF8, -INFINITY);

    // NaN
    test_fp8_to_float("NaN", 0x7F, NAN);

    // Simple normal numbers
    // exp=8 (bias 7) -> 2^1 = 2.0, significand=0 -> 1.0 mantissa -> 2.0
    test_fp8_to_float("2.0", 0x40, 2.0f);
    // exp=7 (bias 7) -> 2^0 = 1.0, significand=0 -> 1.0 mantissa -> 1.0
    test_fp8_to_float("1.0", 0x38, 1.0f);
    // exp=7, significand=4 (100 in binary) -> 1.5 mantissa -> 1.5
    test_fp8_to_float("1.5", 0x3C, 1.5f);
    // exp=8, significand=4 -> 2^1 * 1.5 = 3.0
    test_fp8_to_float("3.0", 0x44, 3.0f);

    // Negative numbers
    test_fp8_to_float("-1.0", 0xB8, -1.0f);
    test_fp8_to_float("-2.0", 0xC0, -2.0f);

    // Subnormal numbers
    // exp=0, significand=1 (smallest subnormal) -> 2^(-6) * 0.125 = 2^(-9)
    test_fp8_to_float("Smallest subnormal", 0x01, 0.001953125f);
    // exp=0, significand=7 (largest subnormal)
    test_fp8_to_float("Largest subnormal", 0x07, 0.013671875f);

    // Largest normal number: exp=14, significand=7 -> 2^7 * 1.875 = 240
    test_fp8_to_float("Largest normal", 0x77, 240.0f);

    printf("\n--- Testing Float to FP8 Conversion ---\n\n");

    // Zero
    test_float_to_fp8("Positive zero", 0.0f, 0x00);
    test_float_to_fp8("Negative zero", -0.0f, 0x80);

    // Infinity
    test_float_to_fp8("Positive infinity", INFINITY, 0x78);
    test_float_to_fp8("Negative infinity", -INFINITY, 0xF8);

    // NaN
    test_float_to_fp8("NaN", NAN, 0x79);

    // Normal numbers
    test_float_to_fp8("1.0", 1.0f, 0x38);
    test_float_to_fp8("2.0", 2.0f, 0x40);
    test_float_to_fp8("1.5", 1.5f, 0x3C);
    test_float_to_fp8("-1.0", -1.0f, 0xB8);
    test_float_to_fp8("-2.0", -2.0f, 0xC0);

    // Overflow (should become infinity)
    test_float_to_fp8("Overflow (500)", 500.0f, 0x78);
    test_float_to_fp8("Overflow (-600)", -600.0f, 0xF8);

    // Underflow (should become zero or subnormal)
    test_float_to_fp8("Underflow (very small)", 0.0001f, 0x00);

    // Value that should round
    test_float_to_fp8("3.0", 3.0f, 0x44);

    printf("\n--- Testing Round-trip Conversion ---\n\n");

    // Test that fp8 -> float -> fp8 preserves the value
    test_roundtrip("Zero", 0x00);
    test_roundtrip("One", 0x38);
    test_roundtrip("Two", 0x40);
    test_roundtrip("1.5", 0x3C);
    test_roundtrip("Negative one", 0xB8);
    test_roundtrip("Positive infinity", 0x78);
    test_roundtrip("Negative infinity", 0xF8);
    test_roundtrip("Subnormal", 0x03);
    test_roundtrip("Largest normal", 0x77);

    printf("\n=== All tests completed ===\n");

    return 0;
}
