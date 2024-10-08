/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 09/30/2024
 * Description: program which reads a ppm file
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"


// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  return NULL;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  FILE* ppmData = fopen(filename, "rb");
    
  if (ppmData == NULL) {
       perror("Error opening file");
       return NULL;
   }
  
  struct ppm_pixel** binary = NULL;
  char buff[1000];
  int index = 0;
  while(fgets(buff, 100, ppmData)){
    if(index == 2){
      sscanf(buff, "%d %d", w, h);
    }
    if(index == 3) break;
    index++;
  }
  
  binary = malloc(sizeof(struct ppm_pixel*)* *h);
  
  for(int i = 0; i < *h; i++){
    binary[i] = malloc(sizeof(struct ppm_pixel)* *w);
    if(binary[i] == NULL){
      perror("Error allocating memory");
      return NULL;
    }
    for(int j = 0; j < *w; j++) fread(&binary[i][j], sizeof(struct ppm_pixel), 1, ppmData);
  }
  
  fclose(ppmData);
  return binary;
  
}