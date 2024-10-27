#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main() {
  
  printf(ANSI_COLOR_RED "OG Shell\n" ANSI_COLOR_RESET);
  char *userInput = NULL;
  char *hold = NULL;
  char *dir = getenv("USER");
  char *host = getenv("HOST");  
  
  using_history();    /* initialize history */
  userInput = readline(hold);
  
  while(strcmp(userInput, "exit") != 0){
    printf("\n");
    add_history(userInput);
    printf("%s:%s-%s$ ", host, dir, userInput);
    free(userInput);
    userInput = readline(hold);
  }

  return 0;

}
