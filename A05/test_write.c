/*----------------------------------------------
 * Author: Oghenefejiro Anigboro 
 * Date: 10/03/2024
 * Description: program which tests writing to a binary file
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);

  // test writing the file to test.ppm, reload it, and print the contents

  write_ppm_2d("test.ppm", pixels, w, h);

  int w2, h2;
  struct ppm_pixel** newPixels = read_ppm_2d("test.ppm", &w2, &h2);
  
  printf("Testing file test.ppm: %d %d\n", w2, h2);
  for(int i = 0; i < w; i++){
    for(int j = 0; j < h; j++){
      printf("(%hhu,%hhu,%hhu) ", newPixels[i][j].red, newPixels[i][j].green, newPixels[i][j].blue);
    }
    printf("\n");
  }

  for(int i = 0; i < w; i++){
    free(pixels[i]);
    free(newPixels[i]);
  }

  free(pixels);
  free(newPixels);
  return 0;
}
