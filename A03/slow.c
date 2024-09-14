/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 9/13/2024
 * Description: program which converts a phrase into an ent phrase
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  short pauseLength;
  char* entPhrase = NULL;
  char buff[32];
  int entIndex = 0;
  
  printf("Pause length: ");
  scanf(" %hd", &pauseLength);
  
  printf("Text: ");
  scanf(" %s", buff);
  
  //entPhrase = malloc(sizeof(char)*3000);
  entPhrase = malloc((sizeof(char) * strlen(buff) * pauseLength) + 1);
  for(int i = 0; i < strlen(buff); i++){
    entPhrase[entIndex] = buff[i];
    for(int j = 0; j < pauseLength; j++){
      entIndex++;
      entPhrase[entIndex] = '.';
    }
    entIndex++;
  }
  entPhrase[entIndex] = '\0';

  printf("%s\n", entPhrase);
  
  free(entPhrase);
  return 0;
}
