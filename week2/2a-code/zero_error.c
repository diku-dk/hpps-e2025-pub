#include <stdio.h>
#include <assert.h>
#include <string.h>

int main() {
    float zero = 0.0f;
    float min_zero = - zero;
 
    printf("zero: %f - Min zero: %f\n", zero, min_zero);

    assert(zero == min_zero);    

    if (memcmp(&zero, &min_zero, sizeof(float)) == 0) {
        printf("At bit level: '0' and '-0' are EQUAL.\n");
    } else {
        printf("At bit level: '0' and '-0' are NOT EQUAL.\n");
    }
    
    return 0;
}
