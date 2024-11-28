/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 11/28/2024
 * Description: Program for implementing dependency listing
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <dirent.h> 
#include "tree.h"

pthread_mutex_t lock;
struct tree_node* root = NULL;

struct thread_data {
  int start, end;
  char **files;
};

void * search(void* userdata) {

  struct thread_data *data = (struct thread_data *) userdata;

  for(int i = data->start; i < data->end; ++i){
    pthread_mutex_lock(&lock);
    root = insert(data->files[i], root);
    pthread_mutex_unlock(&lock);
  }

  return (void*) NULL;
}

void getDependency(char* file){
  FILE *searchFile;
  char line[100];
  char * token = NULL;

  searchFile = fopen(file, "r");
  if(searchFile) printf("%s has the following dependencies\n", file);

  while(fgets(line, 100, searchFile)) {
    if(strstr(line, "#include ")){
      token = strtok(line, " ");
      token = strtok(NULL, " "); // getting second token i.e. dependency
      int i = 0;
      // cleaning dependency
      if (token[i] == '<'){
        char dependency[100];
        while (token[i+1] != '>'){
          dependency[i] = token[i+1];
          ++i;
        } 
        dependency[i] = '\0';
        printf(" %s\n", dependency);
      } else {
        char *dependency = NULL;
        dependency = strtok(token, "\"");
        printf(" %s\n", dependency);
      }
    }
  } 
  fclose(searchFile);
}

int main(int argc, char* argv[]){
  if (argc < 4) {
    printf("usage: ./grep numThreads command directory -name <fileExt>\n");
    return 0;
  }

  int numThreads = atoi(argv[1]);
  char command[5];
  char directory[100];
  char **files = malloc(sizeof(char*)*(100));

  if (numThreads < 1) {
    printf("usage: numThreads > 0\n");
    return 1;
  }

  strcpy(command, argv[2]);
  strncpy(directory, argv[3], 98);
  directory[strlen(directory)] = '/';
  directory[strlen(directory)+1] = '\0';

  int index = 0;
  for(int i = 3; i < argc; ++i){
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
    printf("usage: ./grep numThreads command directory -name <fileExt>\n");
    return 0;
  }

  printf("Processing %d files\n", index);

  float timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
  struct thread_data *data = malloc(numThreads * sizeof(struct thread_data));

  pthread_mutex_init(&lock, NULL);

  if (index >= numThreads){
    for (int i = 0; i < numThreads; i++) {
      data[i].start = i * (int)(index/numThreads);
      if(i == numThreads-1) data[i].end = index;
      else data[i].end = (i+1) * (int)(index/numThreads);
      data[i].files = files;
      printf("Thread %d processing %d files (%d to %d)\n", i, 
                data[i].end - data[i].start, data[i].start+1, data[i].end);
    }

    for(int i = 0; i < numThreads; i++) pthread_create(&threads[i], NULL, search, (void*) &data[i]);
    for(int i = 0; i < numThreads; i++) pthread_join(threads[i], NULL);

    gettimeofday(&tend, NULL);
    timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
    printf("Elapsed time is %f\n", timer);
    pthread_mutex_destroy(&lock);

  } else{
    printf("Number of threads (%d) < number of files (%d) to search\n", numThreads, index);
  }

  char userInput[1024];

  while(1){
    printf("$ ");
    scanf("%[^\n]%*c", userInput);
    if (strcmp(userInput, "quit") == 0) break;
    else if (strcmp(userInput, "list") == 0) printSorted(root);
    else {
      struct tree_node* node = find(userInput, root);
      if (node) getDependency(node->data.name);
      else printf("%s not found\n", userInput);
    }
  }

  for(int i = 0; i < index; ++i) free(files[i]);
  free(files); free(threads); free(data); 
  clear(root);

  return 0;
}
