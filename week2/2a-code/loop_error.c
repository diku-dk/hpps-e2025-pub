#include <stdio.h>
#include <assert.h>

int main() {
    float sum = 0.0f;
    
    // Add 0.1 ten times
    for (int i = 0; i < 10; i++) {
        sum += 0.1f;
    }
    
    printf("Sum after 10 additions: %.10f\n", sum);

    assert(sum == 1.0f);    
    
    return 0;
}
