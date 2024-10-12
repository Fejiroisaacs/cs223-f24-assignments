/*----------------------------------------------
 * Author: Oghenefejiro Anigboro 
 * Date: 10/06/2024
 * Description: This program encodes a users message into a file they give
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int* dec2bin(int num){
  if(num > 255) exit(1);

  int* binArr = malloc(sizeof(int)*8);
  int index = 7;

  while (num > 0){
    binArr[index] = num%2;
    num /= 2;
    --index;
  }

  while(index >= 0){
    binArr[index] = 0;
    --index;
  }

  return binArr;
}

void updateBit(int bit, unsigned char* num){
  int lastBit = *num % 2;
  
  if(bit != lastBit){
    if(*num < 255){ // keep dark colors dark and light colors light
      *num = *num+1; 
    } else *num = *num-1;
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  int w, h, bitsNum;
  struct ppm_pixel** pixels = read_ppm_2d(argv[1], &w, &h);
  
  if(pixels == NULL){
    exit(0);
  }
  
  char* encodedPhrase;
  bitsNum = w*h*3;
  int* bits = malloc(sizeof(int)*bitsNum);
  
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Max number of characters in the image: %d\n", (int)(bitsNum/8)-1);

  // making space for phrase and '\0'
  encodedPhrase = malloc(sizeof(char)*((int)(bitsNum/8))+100);

  printf("Enter a phrase: ");
  fgets(encodedPhrase, (int)(bitsNum/8), stdin);

  // should not be needed -- here because I used scanf to read phrase at first
  if(strlen(encodedPhrase) > (int)(bitsNum/8)-1){
    printf("Invalid phrase. Inputed phrase must be <= %d characters\n",
        (int)(bitsNum/8)-1);
    free(encodedPhrase);
    free(bits);
    for(int i = 0; i < h; i++){
      free(pixels[i]);
    }
    free(pixels);
    exit(0);
  }

  int encodedIndex = 0, currChar = 0;
  printf("Encoding phrase: %s\n", encodedPhrase);
  while(encodedIndex < bitsNum && currChar < strlen(encodedPhrase)){
    int* charConv = dec2bin(encodedPhrase[currChar]);
    for(int i = 0; i < 8; ++i){
      bits[encodedIndex+i] = charConv[i];
    }
    encodedIndex += 8;
    ++currChar;
    free(charConv);
  }

  // ensure index = max index for encoding
  encodedIndex = (strlen(encodedPhrase) * 8);

  int bitIndex = 0, null_ct = 0;
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      // update last bits to encoded values else 0 for null character.
      if(bitIndex >= encodedIndex){
        null_ct++;
        updateBit(0, &pixels[i][j].red);
        if (null_ct == 8) break;
      } else updateBit(bits[bitIndex], &pixels[i][j].red);
      
      if(bitIndex+1 >= encodedIndex){
        null_ct++;
        updateBit(0, &pixels[i][j].green);
        if (null_ct == 8) break;
      } else updateBit(bits[bitIndex+1], &pixels[i][j].green);

      if(bitIndex+2 >= encodedIndex){
        null_ct++;
        updateBit(0, &pixels[i][j].blue);
        if (null_ct == 8) break;
      } else updateBit(bits[bitIndex+2], &pixels[i][j].blue);

      bitIndex+=3;
    }
    if (null_ct == 8) break;
  }
  
  char* output = malloc(sizeof(char)*(strlen(argv[1])+9));
  strncpy(output, argv[1], strlen(argv[1])-4);
  
  // configuring output string
  char* ending = "-encoded.ppm";
  for(int i = 0; i < 12; ++i){
    output[strlen(argv[1])-4+i] = ending[i];
  }
  output[strlen(argv[1])+8] = '\0';
  
  printf("Writing file %s\n", output);
  write_ppm_2d(output, pixels, w, h);
  
  // freeing malloc'd memory
  for(int i = 0; i < h; i++){
    free(pixels[i]);
  }
  free(pixels);
  free(bits);
  free(output);
  free(encodedPhrase);

  return 0;
}
