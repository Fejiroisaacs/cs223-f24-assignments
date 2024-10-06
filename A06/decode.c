/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 10/05/2024
 * Description: This program decodes a hidden message from a ppm file
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int bin2hex(int power){  
  int value = 1;
  for(int i = 0; i < power; i++) value *= 2;
  return value;
}

int* dec2bin(int num){
  if(num > 255) exit(1);

  int* binArr = malloc(sizeof(int)*8);
  int index = 7;

  while (num > 0){
    binArr[index] = num%2;
    num /= 2;
    --index;
  }

  return binArr;
}

int main(int argc, char** argv) {

  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  int w, h, bitsNum;
  struct ppm_pixel** pixels = read_ppm_2d(argv[1], &w, &h);
  

  
  bitsNum = w*h*3;
  int* bits = malloc(sizeof(int)*bitsNum);
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Max number of characters in the image: %d\n", bitsNum/8);
  int bitIndex = 0;
   
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      bits[bitIndex] = pixels[i][j].red % 2;
      bits[bitIndex+1] = pixels[i][j].green % 2;
      bits[bitIndex+2] = pixels[i][j].blue % 2;
      bitIndex+=3;
    }
  }
  
  int i = 0;
  while (i < bitsNum){ 
    if((bitsNum - i) < 8) break;
    int digit = 0;
    for(int j = 0; j < 8; ++j){
      digit += bits[i+j] * bin2hex(7-j);
    }
    i += 8;
    printf("%c", digit);
    
  }
  
  for(int i = 0; i < h; i++){
    free(pixels[i]);
  }

  free(pixels);
  free(bits);
  return 0;
}

