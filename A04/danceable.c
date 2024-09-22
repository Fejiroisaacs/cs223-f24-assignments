/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 09/22/2024
 * Description: Program which uses linked list to 
 * store the songs dataset. Allows user to remove the 
 * most danceable songs in the linked list. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct song{
  int duration;
  float danceability, Energy, Tempo, Valence;
  char artist[128], title[1024];
  struct song* next;
}song;


void printSongs(song* song_list);
void updateSongs(char* line, song* songStruct);
void insert_front(song** head, char* line);
void printTopSong(song* topSong);
void mostDanceable(struct song** head);
void printTopSong(song* topSong);
void freeList(song* head);


/*
 * updates the values in a struct using each line in the csv
 */
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
 * inserts a new node in front of the linked list
 */
void insert_front(struct song** head, char* line){
  struct song* new_song = malloc(sizeof(struct song));
  new_song->next = NULL;
  if (new_song == NULL){
    printf("ERROR: Out of Space!\n");
    exit(1);
  }

  updateSongs(line, new_song);
  new_song->next = *head; 
  *head = new_song; 
}


/*
 * finds the most danceable song in the linked list, prints its value and removes it from memory
 */
void mostDanceable(struct song** head){
  song* topSong = *head;
  song* topPrev = NULL;
  song* prev = NULL;

  for(song* curr = *head; curr != NULL; curr = curr->next){
    if (curr->danceability > topSong->danceability){
      topSong = curr;
      topPrev = prev;
    }
    prev = curr;
  }

  if (topSong->next != NULL){
    if (topPrev != NULL){
      topPrev->next = topSong->next;
    }else{
      *head = topSong->next;
    }
  }else{
    if (topPrev != NULL){
      topPrev->next = NULL;
    } else{
      printf("No songs left\n");
      exit(0);
    }
  }

  printf("-------------------------- Most danceable ----------------------\n");
  printTopSong(topSong);
  printf("----------------------------------------------------------------\n");
  free(topSong);

}


/*
 * takes in a songs struct array and prints the values as a table
 */
void printSongs(song* song_list){
  short min, sec;
  short i = 0;
  for (song* curr = song_list; curr != NULL; curr = curr->next){
    min = (curr->duration * 0.001) / 60;
    sec = (int) (curr->duration * 0.001) % 60;
    printf("%d) %-25s\tartist: %-20s (%d:%02d) d: %.02f e: %.03f "
      "t: %.03f v: %.03f\n", i, curr->title, curr->artist, min, sec, 
        curr->danceability,curr->Energy, curr->Tempo, curr->Valence);
    i++;

  }
  printf("\n\nDataset contains %d songs\n", i);
  printf("=======================\n\n");
}


/*
 * prints the value of the top danceable song in the struct
 */
void printTopSong(song* topSong){
  short min, sec;
  min = (topSong->duration * 0.001) / 60;
  sec = (int) (topSong->duration * 0.001) % 60;
  printf("%-25s\tartist: %-20s (%d:%02d) d: %.02f e: %.03f t: %.03f v: %.03f\n",
      topSong->title, topSong->artist, min, sec, topSong->danceability,
      topSong->Energy, topSong->Tempo, topSong->Valence);
}


/*
 * clears the allocated memory for each song struct node in the linked list
 */
void freeList(song* head){
  struct song* tmp;
  while (head != NULL){
    tmp = head;
    head = head->next;
    free(tmp);
  }
}


int main() {
  FILE *songData;  
  song *head = NULL;
  char line[100];
  char userInput[2];
  songData = fopen("songlist.csv", "r");

  fgets(line, 100, songData); // reading out column titles
  while(fgets(line, 100, songData)) {
    insert_front(&head, strtok(line, ",")); 
  } 
  
  printSongs(head);
  printf("\nPress 'd' to show the most danceable song (any other key to quit): ");
  scanf(" %s", userInput);

  while(strcmp(userInput, "d") == 0){
    mostDanceable(&head);
    printf("\nPress 'd' to show the most danceable song (any other key to quit): ");
    scanf(" %s", userInput);
    printSongs(head);
  }

  freeList(head);
  fclose(songData);

  return 0;
}
