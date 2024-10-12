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

int main(int argc, char** argv) {

  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }
  
  int w, h, bitsNum;
  struct ppm_pixel** pixels = read_ppm_2d(argv[1], &w, &h);
  
  if(pixels == NULL){
    exit(0);
  } 

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
    if(digit%256 == 0) break;
    printf("%c", digit%256); // keep within ascii characters
    
  }

  printf("\n"); 
  for(int i = 0; i < h; i++){
    free(pixels[i]);
  }

  free(pixels);
  free(bits);
  return 0;
}

