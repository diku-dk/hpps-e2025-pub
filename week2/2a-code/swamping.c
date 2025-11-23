#include <stdio.h>
#include <assert.h>

int main() {
    float l = 10000000.0f; 
    float s = 0.0000001f;  
    
    float sum = l + s;
    
    printf("Large number: %.10f\n", l);
    printf("Small number: %.10f\n", s);
    printf("Sum:          %.10f\n", sum);

    assert(l == sum);

    return 0;
}
