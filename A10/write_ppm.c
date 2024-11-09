/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 10/03/2024
 * Description: Program which writes binary data
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {

}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {
  
  FILE* binFile = fopen(filename, "wb"); 

  char dimensions[20]; 
  sprintf(dimensions, "%d %d", w, h);

  fprintf(binFile, "%s", "P6\n");
  fprintf(binFile, "%s", "#Created by GIMP version 2.10.24 PNM plug-in\n");
  fprintf(binFile, "%s", dimensions);
  fprintf(binFile, "%s", "\n255\n");
  
  for(int i = 0; i < w; i++){
   for(int j = 0; j < h; j++){
    fwrite(&pixels[i][j], sizeof(struct ppm_pixel), 1, binFile);
   }
  }  
  
  fclose(binFile);

}
