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
  scanf(" %[^\n]%*c", buff);
  
  entPhrase = malloc((sizeof(char) * strlen(buff) + 1) * pauseLength);
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
  printf("%ld", strlen(entPhrase));
  free(entPhrase);
  return 0;
}
