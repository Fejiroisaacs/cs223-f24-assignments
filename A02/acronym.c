/***************************************************
 * acronym.c
 * This program converts the users input into an acronym.
 * @author: Oghenefejiro Anigboro
 * @version: September 6th, 2024
 */

#include <stdio.h>
#include <string.h>

int main() {
  short i, acronymIndex = 0;
  short ascii;
  char acronym[512], phrase[1024];

  printf("Enter a phrase: ");
  scanf("%[^\n]%*c", phrase);
  
  for(i = 0; i < strlen(phrase); i++){
    ascii = (short) phrase[i];

    /* checks if a character is upper case and it's the first element in the 
     phrase or its previous element was a space */
    if ((ascii >= 65 && ascii <= 90) && (i == 0 || (short) phrase[i-1] == 32)){
      acronym[acronymIndex] = phrase[i];
      acronymIndex++;
    }
  }
  acronym[acronymIndex] = '\0';
  printf("Your acronym is %s\n", acronym);

  return 0;
}
