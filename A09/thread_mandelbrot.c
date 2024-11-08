/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 11/07/2024
 * Description: Program for computing the Mandelbrot set using 4 threads
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include "read_ppm.h"
#include "write_ppm.h"

struct thread_data {
  int x1, y1, x2, y2, size, maxIterations;
  float xmax, xmin, ymin, ymax;
  struct ppm_pixel* palette;
  struct ppm_pixel** pixels;
};

void* compute_pixel(void* userdata){
  pthread_t tid = pthread_self();
  struct thread_data *data = (struct thread_data *) userdata;
  printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d,%d)\n", 
                                    tid, data->x1, data->x2, data->y1, data->y2);

  for(int i = data->x1; i < data->x2; i++){
    for(int j = data->y1; j < data->y2; j++){
      double xfrac = (double)i / data->size;
      double yfrac = (double)j / data->size;
      double x0 = data->xmin + xfrac * (double)(data->xmax - data->xmin);
      double y0 = data->ymin + yfrac * (double)(data->ymax - data->ymin);

      double x = 0;
      double y = 0;
      int iter = 0;
      while (iter < data->maxIterations && x*x + y*y < 2*2){
        double xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      
      struct ppm_pixel color;
      if (iter < data->maxIterations){
        color = data->palette[iter];
        data->pixels[i][j] = color;
      }
    }
  }
  printf("Thread %lu) finished\n", tid);
  return (void*) NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  struct ppm_pixel* palette;
  struct ppm_pixel black = {0, 0, 0};
  struct ppm_pixel** pixels;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }

  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // generate pallet
  srand(time(0));
  palette = malloc(sizeof(struct ppm_pixel)*maxIterations);
  for(int i = 0; i < maxIterations; i++){
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  // intialize pixels to all 0
  pixels = malloc(sizeof(struct ppm_pixel*) * size);
  for(int i = 0; i < size; i++){
    pixels[i] = malloc(sizeof(struct ppm_pixel) * size);
    if(pixels[i] == NULL){
      perror("Error allocating memory");
      return 1;
    }
    for(int j = 0; j < size; j++){
      pixels[i][j] = black;
    }
  }

  // compute image
  double timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  pthread_t threads[4];
  struct thread_data data[4];
  int quadrants[4][4] = {
    {0, size/2, 0, size/2},
    {size/2, size, 0, size/2},
    {0, size/2, size/2, size},
    {size/2, size, size/2, size}
  };

  for (int i = 0; i < numProcesses; i++) {
    data[i].x1 = quadrants[i][0];
    data[i].x2 = quadrants[i][1];
    data[i].y1 = quadrants[i][2];
    data[i].y2 = quadrants[i][3];
    data[i].palette = palette;
    data[i].pixels = pixels;
    data[i].size = size;
    data[i].maxIterations = maxIterations;
    data[i].xmax = xmax;
    data[i].xmin = xmin;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    pthread_create(&threads[i], NULL, compute_pixel, (void*) &data[i]);
  }

  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
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
