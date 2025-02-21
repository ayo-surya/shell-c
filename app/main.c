#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/wait.h>




void execute(int argc, char **argv){
  pid_t pid = fork();
  if (pid == 0)
  {
    execvp(argv[0], argv);
    printf("%s: command not found\n", argv[0]);
    exit(1);
  }
  else if(pid < 0)
  {
    perror("fork");
  }
  else {
    int status;
    waitpid(pid, &status, 0);
  }
  
}

void get_args(char *line){
  // printf("from the get_args function line is: %s\n", line);
  char *tokens[10] = {0};
  char *token = strtok(line, " ");
  int partition = 0;

  while (token != NULL)
  {
    tokens[partition] = token;
    partition++;
    token = strtok(NULL, " ");
  }
  tokens[partition] = NULL;
  execute(partition+1, tokens);
}
  
int changeIntoHomeDir(){
  char *env_home = getenv("HOME");
  if (chdir(env_home) == 0){
    return 1;
  }
  return 0;
}

int main() {

  setbuf(stdout, NULL); // setting buffer to NULL to throw the output immediately
  
  while (1) // continuously taking the input and excecuting it line after line
  {
    printf("$ "); // this is just for letting the user know that you have to enter some input
    fflush(stdout);

    char input[100];
    fgets(input, 100, stdin);  // reads the input until it encounters "\n"
    input[strlen(input)-1] = '\0';  // clears the "\n" which is built-in in fgets function

    
    
    char cmd[5];
    strncpy(cmd, input, 4);
    char *args = NULL;

    for(int i = 0; i < strlen(input)+1; i++){ // loop for intializing the args pointer to the desired location
      if(input[i] == ' '){
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
      if(strcmp("echo", args) == 0 || strcmp("exit", args) == 0 || strcmp("type", args) == 0 || strcmp(args, "pwd") == 0 || strcmp(args, "cd") == 0){
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
        // printf("going into the get_args function: ");
        printf("%s: not found\n", args);
      }
      continue;
    }
    else if (strcmp("pwd", input)== 0)
    {
      char cwd[1024];
      if(getcwd(cwd, sizeof(cwd)) != NULL)
      {
        printf("%s\n", cwd);
      }
      else{
        perror("getcwd() error");
        return 1;
      }
    }
    else if(strncmp("cd", input, 2) == 0){
      if (strcmp(args, "~") == 0){
        if (changeIntoHomeDir()){
          continue;
        }
      }
      
      else if (chdir(args) == 0){
        continue;
      }
      else{
        printf("cd: %s: No such file or directory\n", args);
      }
      continue;
    }
    else{
      get_args(input);; // every other command shall print this statement
    }
  }
  
  return 0;
}
