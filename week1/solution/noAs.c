// noAs solution
#include <stdio.h>

int main(int argc, char* argv[]){
  if (argc != 2) {
    printf("Wrong number of arguments!\n");
    return 1;
  } else {
    if (argv[1][0] == 'A') {
      printf("Arguments should not start with A\n");
    } else {
      printf("The argument is %s\n", argv[1]);
    }
  }
  
}
