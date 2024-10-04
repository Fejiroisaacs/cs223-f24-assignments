/*----------------------------------------------
 * Author: Oghenefejiro Anigboro 
 * Date: 10/2/2024
 * Description: Program which reads and prints the values of a binary file
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w, h;
  
  struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);
  // todo: print out the grid of pixels
  for(int i = 0; i < w; i++){
   for(int j = 0; j < h; j++){
      printf("(%hhu,%hhu,%hhu) ", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
    }
    printf("\n");
  }
  
  for(int i = 0; i < w; i++){
    free(pixels[i]);
  }

  free(pixels);
  return 0;
}

