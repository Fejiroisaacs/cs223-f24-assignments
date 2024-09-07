/***************************************************
 * songs.c
 * This program 
 * @author: Oghenefejiro Anigboro
 * @version: September 6th, 2024
 */

#include <stdio.h>
#include <string.h>

/*
 * Struct which saves info about a song
 */
struct song{
  int duration;
  float danceability;
  char artist[128], title[1024];
};

void printSongs(struct song songs[3]){
  int min, sec, i;
  for (i = 0; i < 3; i++){
    min = songs[i].duration / 60;
    sec = songs[i].duration % 60;
    printf("%d) %-25s \tartist: %-20s \tduration: %2d:%-2d \tdanceability: %.02f\n", i, songs[i].title,
    songs[i].artist, min, sec, songs[i].danceability);
  }
}

int editSongs(struct song songs[3]){
  int songId;
  char option[32];

  printf("Enter a song id to edit [0,1,2]: ");
  scanf(" %d", &songId);

  if (songId > 2 || songId < 0){
    printf("Invalid Choice!\n");
    return 1; // function terminates if invalid option is selected
  }
  
  printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
  scanf(" %s", option);

  if (strcmp("artist", option) == 0){
    char newArtist[64];
    printf("Enter an artist: ");
    scanf(" %[^\n]%*c", songs[songId].artist);
    printSongs(songs);
  } else if (strcmp("title", option) == 0){

  } else if (strcmp("duration", option) == 0){

  } else if (strcmp("danceability", option) == 0){

  } else {
    printf("Invalid choice\n");
    return 1;
  }
  return 0;
}

int main() {
  printf("Welcome to Fejiro Struct's Song List.\n");
  struct song songsArray[3];

  strcpy(songsArray[0].title, "Love Me JeJe");
  strcpy(songsArray[0].artist, "Tems");
  songsArray[0].danceability = 0.9;
  songsArray[0].duration = 178;

  strcpy(songsArray[1].title, "Dance with My Father");
  strcpy(songsArray[1].artist, "Luther Vandross");
  songsArray[1].danceability = 0.6;
  songsArray[1].duration = 266;

  strcpy(songsArray[2].title, "WE PRAY");
  strcpy(songsArray[2].artist, "Coldplay");
  songsArray[2].danceability = 0.3;
  songsArray[2].duration = 233;

  printSongs(songsArray);
  printf("\n\n=======================\n\n");
  editSongs(songsArray);
  return 0;
}
