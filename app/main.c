#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  // Flush after every printf
  setbuf(stdout, NULL);

  // Uncomment this block to pass the first stage
  
  char input[100];
  while (1)
  {
    printf("$ ");

    fgets(input, 100, stdin);
    input[strlen(input)-1] = '\0';

    if(!strcmp(input, "exit 0")){
        exit(0);
    }
    
    printf("%s: command not found\n", input);
  }
  
  return 0;
}
