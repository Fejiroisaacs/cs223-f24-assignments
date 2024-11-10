/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 11/07/2024
 * Description: Program for computing the Mandelbrot set using a single thread
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  struct ppm_pixel* palette;
  struct ppm_pixel black = {0, 0, 0};
  struct ppm_pixel** pixels;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);
  
  
  // todo: your work here
  // generate pallet
  srand(time(0));
  palette = malloc(sizeof(struct ppm_pixel)*maxIterations);
  unsigned char basered = rand() % 255;
  unsigned char basegreen = rand() % 255;
  unsigned char baseblue = rand() % 255;
  for(int i = 0; i < maxIterations; i++){
    palette[i].red = basered + rand() % 100 - 50;
    palette[i].green = basegreen + rand() % 100 - 50;
    palette[i].blue = baseblue + rand() % 100 - 50;
  }
  // compute image
  pixels = malloc(sizeof(struct ppm_pixel*) * size);
  for(int i = 0; i < size; i++){
    pixels[i] = malloc(sizeof(struct ppm_pixel) * size);
    if(pixels[i] == NULL){
      perror("Error allocating memory");
      return 1;
    }
  }

  double timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      double xfrac = (double)i / size;
      double yfrac = (double)j / size;
      double x0 = xmin + xfrac * (double)(xmax - xmin);
      double y0 = ymin + yfrac * (double)(ymax - ymin);

      double x = 0;
      double y = 0;
      int iter = 0;
      while (iter < maxIterations && x*x + y*y < 2*2){
        double xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      
      struct ppm_pixel color;
      if (iter < maxIterations){ // escaped
        color = palette[iter];
      } else {
        color = black;
      }
      pixels[j][i] = color;
    }
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);
  
  char *filename = malloc(sizeof(char)*50);
  char *filestart = "mandelbrot-";
  char *fileend = ".ppm";
  
  int index;
  for(index = 0; index < strlen(filestart); index++){
    filename[index] = filestart[index];
  }

  char sizeStr[5];
  sprintf(sizeStr, "%d", size);
  
  for(int i = 0; i < strlen(sizeStr); i++){
    filename[index] = sizeStr[i];
    ++index;
  }
  
  filename[index] = *"-";
  
  char timeStamp[11];
  sprintf(timeStamp, "%ld", time(0));
  for(int i = 0; i < strlen(timeStamp); i++){
    ++index;
    filename[index] = timeStamp[i];
  }
  
  for(int i = 0; i < strlen(fileend); i++){
    ++index;
    filename[index] = fileend[i];
  }
  
  filename[index+1] = *"\0";

  printf("Writing file: %s\n", filename);
  write_ppm_2d(filename, pixels, size, size);

  // freeing malloc'd memory
  for(int i = 0; i < size; i++){
    free(pixels[i]);
  }
  free(pixels);
  free(palette);
  free(filename);
}
