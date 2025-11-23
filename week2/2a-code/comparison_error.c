#include <stdio.h>
#include <assert.h>

int main() {
    double a = 0.1;
    double b = 0.2;
    double sum = a + b;
    
    printf("a = %.20f\n", a);
    printf("b = %.20f\n", b);
    printf("sum (a + b) = %.20f\n", sum);
    printf("0.3 = %.20f\n\n", 0.3);

    assert(sum == 0.3);
    
    return 0;
}
