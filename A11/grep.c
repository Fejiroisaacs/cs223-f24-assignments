/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 11/25/2024
 * Description: Program implementing grep
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <dirent.h> 

pthread_mutex_t lock;

struct thread_data {
  int start, end, id, count;
  char **files;
  char *keyword;
};

void * search(void* userdata) {
  pthread_t tid = pthread_self();
  struct thread_data *data = (struct thread_data *) userdata;
  FILE *searchFile;
  char line[100];
  data->count = 0;
  
  for(int i = data->start; i < data->end; ++i){
    searchFile = fopen(data->files[i], "r");
    while(fgets(line, 100, searchFile)) {
      if(strstr(line, data->keyword)){
        data->count++;
        pthread_mutex_lock(&lock);
        printf("%d) %s:%s", data->id, data->files[i], line);
        pthread_mutex_unlock(&lock);
      }
    } 
    fclose(searchFile);
  }

  return (void*) NULL;
}

int main(int argc, char* argv[]) {
  if (argc < 5) {
    printf("usage: ./grep numThreads keyword command directory -name <fileExt>\n");
    return 0;
  }

  const int numThreads = atoi(argv[1]);
  char keyword[100];
  char command[5];
  char directory[100];
  char **files = malloc(sizeof(char*)*(100));

  strcpy(keyword, argv[2]);
  strcpy(command, argv[3]);
  strncpy(directory, argv[4], 98);
  directory[strlen(directory)] = '/';
  directory[strlen(directory)+1] = '\0';

  int index = 0;
  for(int i = 4; i < argc; ++i){
    if (strcmp(argv[i], "-name") == 0){
      if (i+1 < argc){
        if(strstr(argv[i+1], "*")){
          DIR *d;
          struct dirent *dir;
          d = opendir(directory);
          char ext[10];
          strncpy(ext, argv[i+1]+1, strlen(argv[i+1]));
          if (d) {
            while ((dir = readdir(d)) != NULL) {
              if(strstr(dir->d_name, ext)){
                files[index] = malloc(sizeof(char)*100); 
                sprintf(files[index], "%s%s", directory, dir->d_name);
                ++index;
              }
            }
            closedir(d);
          }
        } else{
          files[index] = malloc(sizeof(char)*100); 
          sprintf(files[index], "%s/%s", directory, argv[i]); ++index;
        }
      }
    }
  }

  if (index < 1) {
    printf("usage: ./grep numThreads keyword command directory -name <fileExt>\n");
    return 0;
  }

  printf("Searching %d files for keyword: public\n", index);

  float timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  pthread_t threads[numThreads];
  struct thread_data data[numThreads];

  pthread_mutex_init(&lock, NULL);

  if (index > numThreads){
    for (int i = 0; i < numThreads; i++) {
      data[i].id = i;
      data[i].keyword = keyword;
      data[i].start = i * (int)(index/numThreads);
      if(i == numThreads-1) data[i].end = index;
      else data[i].end = (i+1) * (int)(index/numThreads);
      data[i].files = files;
      printf("Thread %d searching %d files (%d to %d)\n", i, 
                data[i].end - data[i].start, data[i].start+1, data[i].end);
    }

    for(int i = 0; i < numThreads; i++) pthread_create(&threads[i], NULL, search, (void*) &data[i]);
    for(int i = 0; i < numThreads; i++) pthread_join(threads[i], NULL);
    for(int i = 0; i < numThreads; i++) {
      printf("Thread %d found %d lines containing keyword: %s\n", 
                        data[i].id, data[i].count, keyword);
    }

    gettimeofday(&tend, NULL);
    timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
    printf("Elapsed time is %f\n", timer);
    pthread_mutex_destroy(&lock);

  } else{
    printf("Number of threads (%d) < number of files (%d) to search", numThreads, index);
  }

  for(int i = 0; i < index; ++i) free(files[i]);
  free(files);

  return 0;
}
