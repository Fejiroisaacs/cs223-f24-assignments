/*----------------------------------------------
 * Author: Oghenefejiro Anigboro 
 * Date: 10/28/2024
 * Description: warm up program for learning how to fork
 ---------------------------------------------*/
#include <stdio.h>
#include  <string.h>
#include  <sys/types.h>
#include <unistd.h>

int main() {
  pid_t pid;
  
  printf("%d] A0\n", getpid());
  fflush(stdout);
  pid = fork();

  if (pid == 0){
    printf("%d] B1\n", getpid());
    fflush(stdout);
  } else {
    printf("%d] B0\n", getpid());
    fflush(stdout);
    pid = fork();
    if (pid == 0){
      printf("%d] C1\n", getpid());
      fflush(stdout);
    } else {
      printf("%d] C0\n", getpid());
      fflush(stdout);
    }
  }
  
  printf("%d] Bye\n", getpid());

  return 0;

}
