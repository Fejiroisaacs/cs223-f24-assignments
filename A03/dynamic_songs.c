/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 09/14/2024
 * Description: This program serves as a database for Fejiro's songs
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct song{
  int duration;
  float danceability, Energy, Tempo, Valence;
  char artist[128], title[1024];
}song;

void updateSongs(char* line, song* songStruct){
  short index = 0;
  while(line != NULL){
    if(index == 0){
      strcpy(songStruct->title, line);
    } else if(index == 1) {
      strcpy(songStruct->artist, line);
    } else if(index == 2) {
      songStruct->duration = atoi(line);
    } else if(index == 3) {
      songStruct->danceability = atof(line);
    } else if(index == 4) {
      songStruct->Energy = atof(line);
    } else if(index == 5) {
      songStruct->Tempo = atof(line);
    } else if(index == 6) {
      songStruct->Valence = atof(line);
    }
    index++;
    line = strtok(NULL, ",");
  }
}

/*
 * takes in a songs struct array and prints the values as a table
 */
void printSongs(struct song songs[]){
  short int min, sec, i;
  for (i = 0; i < 18; i++){
    min = (songs[i].duration * 0.001) / 60;
    sec = (int) (songs[i].duration * 0.001) % 60;
    printf("%d) %-25s\tartist: %-20s\tduration: %2d:%02d \t d: %.02f \t "
        "e: %.03f \t t: %.03f \t v: %.03f\n",
        i, songs[i].title, songs[i].artist, min, sec, songs[i].danceability,
        songs[i].Energy, songs[i].Tempo, songs[i].Valence);
  }
}

int main() {
  FILE *songData;  
  song *songArray = NULL;
  char line[100];
  short index = -2;
  songData = fopen("songlist.csv", "r");
  
  while(fgets(line, 100, songData)) {
    if(index == -2){
      songArray = malloc(sizeof(song)*atoi(line));
      index+=2;
      fgets(line, 100, songData); // reading out column titles
      continue;
    }
    updateSongs(strtok(line, ","), &songArray[index]);
    index++;
  }
  
  printf("Welcome to Oghenefejiro Anigboro's Danceability Directory.");
  printSongs(songArray);
  free(songArray);
  fclose(songData);

  return 0;
}
