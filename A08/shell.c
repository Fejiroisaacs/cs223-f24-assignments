#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

int main() {
  
  char* args[] = {NULL};
  char *hold = " ";
  char *userInput = NULL;
  char dir[1024];
  char hostname[1024];
  struct passwd *pws;

  printf(RED "Welcome to my custom OG Shell " RESET);
  printf(CYAN "<3 -- :)\n" RESET);

  pws = getpwuid(geteuid());
  gethostname(hostname, 1023);
  getcwd(dir, 1023);
  hostname[1023] = '\0';
  dir[1023] = '\0';

  using_history(); 

  while(1){
    printf("\033[5;93m%s\033[0m" "@" MAGENTA"%s:" CYAN"%s" RESET ":) ", pws->pw_name, hostname, dir);
    userInput = readline(hold);

    if (userInput == NULL || strcmp(userInput, "exit") == 0) {
      free(userInput);
      break;
    }

    if(strlen(userInput) > 0) {
      add_history(userInput);
    }

    int index = 0;
    char *token = strtok(userInput, " ");
    while(token != NULL) {
      args[index] = token;
      token = strtok(NULL, " ");
      ++index;
    }

    args[index] = NULL;

    pid_t pid = fork();
    int status;

    if(pid == 0){
      int ret = execvp(args[0], args);
      if(ret < 0){
        printf(RED "%s:Command not found\n" RESET, args[0]);
      }
      exit(0);
    } else {
      pid_t w = waitpid(pid, &status, 0);
      if (w == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
      }
    }

    free(userInput);
    getcwd(dir, 1023);

  }

  return 0;
}
