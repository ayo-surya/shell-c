#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stddef.h>

int main() {

  setbuf(stdout, NULL); // setting buffer to NULL to throw the output immediately
  
  while (1) // continuously taking the input and excecuting it line after line
  {
    printf("$ "); // this is just for letting the user know that you have to enter some input

    char input[100];
    fgets(input, 100, stdin);  // reads the input until it encounters "\n"
    input[strlen(input)-1] = '\0';  // clears the "\n" which is built-in in fgets function
    
    char *cmd = input;
    char *args = NULL;

    for(int i = 0; i < strlen(input)+1; i++){
      if(input[i] == ' '){
        input[i] = '\0';
        args = &input[i+1];
        break;
      }
    }
    
    if(!strcmp(cmd, "exit")){  // exiting with 0 when the "exit 0" input is given which will terminate the program                     
        exit(0);
    } 

    else if(!strcmp(cmd, "echo")){  // if the input starts with "echo" then the string after shall be printed 
      printf("%s\n", args);
    }

    else if(strcmp("type", cmd) == 0){
      if(strcmp("echo", args) == 0 || strcmp("exit", args) == 0 || strcmp("type", args) == 0){
        printf("%s is a shell builtin\n", args);
        continue;
      }
    
      char *env_path = getenv("PATH");
      if (env_path == NULL) {
        printf("PATH environment variable not found\n");
        continue;
      }
      
      char *PATH = malloc(strlen(env_path) + 1);  // +1 for null terminator
      if (PATH == NULL) {
        printf("Memory allocation failed\n");
        continue;
      }
      
      strcpy(PATH, env_path);
      int found = 0;  // Flag to track if we found the command
      
      char *dirpath = strtok(PATH, ":");
      
      while (dirpath && !found) {
        DIR *directory = opendir(dirpath);
        
        if (directory == NULL) {
          dirpath = strtok(NULL, ":");
          continue;
        }
    
        struct dirent *entry;
        
        while((entry = readdir(directory)) != NULL) {
          if (strcmp(entry->d_name, args) == 0) {
            printf("%s is %s/%s\n", args, dirpath, entry->d_name);
            found = 1;
            break;
          }
        }
        
        closedir(directory);
        if (!found) {
          dirpath = strtok(NULL, ":");
        }
      }
      
      free(PATH);
      
      if (!found) {
        printf("%s: not found\n", args);
      }
      continue;
    }
    else{
      printf("%s: command not found\n", input); // every other command shall print this statement
    }
  }
  
  return 0;
}
