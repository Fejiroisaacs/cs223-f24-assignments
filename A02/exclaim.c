/***************************************************
 * exclaim.c
 * @author: Oghenefejiro Anigboro
 * @version: September 6th, 2024 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
  srand(time(NULL));

  char buff[32], short i, short ascii;
  char exclaimChars[4] = {'@', '!', '#', '*'};

  printf("Enter a word: ");
  scanf(" %s", buff);

  for(i = 0; i < strlen(buff); i++){
    ascii = (int) buff[i];
    if (ascii >= 97 && ascii <= 122){
      buff[i] = exclaimChars[rand() % 4]; // random int from 0 to 3
    }
  }

  printf("OMG! %s\n", buff);

  return 0;
}
