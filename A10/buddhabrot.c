/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 11/09/2024
 * Description: Program for making an PPM image of the Buddhabrot using multiple threads 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include "read_ppm.h"
#include "write_ppm.h"

int MAX_COUNT = 0;
pthread_barrier_t barrier;
pthread_mutex_t lock; 

struct thread_data {
  int x1, y1, x2, y2, size, maxIterations;
  float xmax, xmin, ymin, ymax;
  int** membership;
  int** counts;
  struct ppm_pixel** pixels;
};

void * start(void* userdata) {

  // perform step 1
  pthread_t tid = pthread_self();
  int max_count = 0;
  struct thread_data *data = (struct thread_data *) userdata;
  printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d,%d)\n", 
                                    tid, data->x1, data->x2, data->y1, data->y2);

  for(int i = data->x1; i < data->x2; i++){
    for(int j = data->y1; j < data->y2; j++){
      float xfrac = (float)i / data->size;
      float yfrac = (float)j / data->size;
      float x0 = data->xmin + xfrac * (float)(data->xmax - data->xmin);
      float y0 = data->ymin + yfrac * (float)(data->ymax - data->ymin);

      float x = 0;
      float y = 0;
      int iter = 0;
      while (iter < data->maxIterations && x*x + y*y < 2*2){
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      
      struct ppm_pixel color;
      if (iter < data->maxIterations){
        data->membership[i][j] = 0;
      } else {
        data->membership[i][j] = 1;
      }
    }
  }
  
  // perform step 2
  for(int i = data->x1; i < data->x2; i++){
    for(int j = data->y1; j < data->y2; j++){
      if(data->membership[i][j]) continue;

      float xfrac = (float)i / data->size;
      float yfrac = (float)j / data->size;
      float x0 = data->xmin + xfrac * (float)(data->xmax - data->xmin);
      float y0 = data->ymin + yfrac * (float)(data->ymax - data->ymin);

      float x = 0;
      float y = 0;
      while (x*x + y*y < 2*2){
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        int yrow = round(data->size * (y - data->ymin)/(data->ymax - data->ymin));
        int xcol = round(data->size * (x - data->xmin)/(data->xmax - data->xmin));
        if (yrow < 0 || yrow >= data->size) continue; // out of range
        if (xcol < 0 || xcol >= data->size) continue; // out of range
        ++data->counts[yrow][xcol];
        if (data->counts[yrow][xcol] > max_count) max_count = data->counts[yrow][xcol];
      }
    }
  }

  pthread_mutex_lock(&lock);
  if (max_count > MAX_COUNT) MAX_COUNT = max_count;
  pthread_mutex_unlock(&lock);

  // use a thread barrier to wait for all threads to finish steps 1 and 2
  pthread_barrier_wait(&barrier);

  // perform step 3
  float gamma = 0.681;
  float factor = 1.0/gamma;
  for(int i = data->x1; i < data->x2; i++){
    for(int j = data->y1; j < data->y2; j++){
      float value = 0;

      if(data->counts[i][j] > 0){
        value = log(data->counts[i][j]) / log(MAX_COUNT);
        value = pow(value, factor);
      }
      struct ppm_pixel color;
      color.red = value * 255;
      color.green = value * 255;
      color.blue = value * 255;
      data->pixels[i][j] = color;
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

  int** membership;
  int** counts;
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
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here

  srand(time(0));
  membership = malloc(sizeof(int *)*maxIterations);
  counts = malloc(sizeof(int *)*maxIterations);

  // intialize pixels to all 0
  pixels = malloc(sizeof(struct ppm_pixel*) * size);
  for(int i = 0; i < size; i++){
    pixels[i] = malloc(sizeof(struct ppm_pixel) * size);
    membership[i] = malloc(sizeof(int) * size);
    counts[i] = malloc(sizeof(int) * size);
    if(pixels[i] == NULL || membership[i] == NULL || counts == NULL){
      perror("Error allocating memory");
      return 1;
    }
    for(int j = 0; j < size; j++){
      counts[i][j] = 0;
    }
  }

  // compute image
  float timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  pthread_t threads[4];
  struct thread_data data[4];
  pthread_barrier_init(&barrier, NULL, numProcesses);
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
    data[i].membership = membership;
    data[i].counts = counts;
    data[i].pixels = pixels;
    data[i].size = size;
    data[i].maxIterations = maxIterations;
    data[i].xmax = xmax;
    data[i].xmin = xmin;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    pthread_create(&threads[i], NULL, start, (void*) &data[i]);
  }

  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed buddhabrot set (%dx%d) in %f seconds\n", size, size, timer);
  pthread_barrier_destroy(&barrier);

  char *filename = malloc(sizeof(char)*50);
  char *filestart = "buddhabrot-";
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
    free(membership[i]);
    free(counts[i]);
    free(pixels[i]);
  }
  free(pixels);
  free(membership);
  free(counts);
  free(filename);
}
