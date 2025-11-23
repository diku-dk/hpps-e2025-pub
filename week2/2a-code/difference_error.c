#include <stdio.h>
#include <assert.h>

void test_float_difference(void) {
    float a = 123456.789f;
    float b = 123456.780f;
    float diff = a - b;
    
    printf("a = %.10f\n", a);
    printf("b = %.10f\n", b);
    printf("Difference: %.10f\n", diff);

    assert(diff == 0.009f);
}

void test_double_difference(void) {
    double a = 123456.789;
    double b = 123456.780;
    double diff = a - b;
    
    printf("\nDouble Difference: %.10f\n", diff);

    assert(diff == 0.009f);
}


void test_difference(void) {
    double a = 1234.56789;
    double b = 9876.54321;
    double diff = a - b;
    assert(diff == 8641.97532);
}

int main(void) {
    test_float_difference();
    // test_double_difference();
    // test_difference();

    printf("All tests passed.\n");
    
    return 0;
}
