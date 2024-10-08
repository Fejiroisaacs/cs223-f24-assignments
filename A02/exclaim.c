/***************************************************
 *
 * exclaim.c
 * This program converts the users input into an exclamation.
 * @author: Oghenefejiro Anigboro
 * @version: September 6th, 2024 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
  srand(time(NULL));

  char buff[32];
  short ascii;
  char exclaimChars[4] = {'@', '!', '#', '*'}; // no need for null char here

  printf("Enter a word: ");
  scanf(" %s", buff);

  for(int i = 0; i < strlen(buff); i++){
    ascii = (short) buff[i];
    if (ascii >= 97 && ascii <= 122){
      buff[i] = exclaimChars[rand() % 4]; // random int from 0 to 3
    }
  }

  printf("OMG! %s\n", buff);

  return 0;
}
