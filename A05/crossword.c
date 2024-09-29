/*----------------------------------------------
 * Author: Oghenefejiro Anigboro
 * Date: 09/28/2024
 * Description: program that generates a crossword using two words from a user. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printMatrix(char** matrix, int nrows, int ncols);
int* commonLetter(char* word1, char* word2);
void makeMatrix(char* word1, char* word2);

void printMatrix(char **matrix, int nrows, int ncols){
  for(int i = 0; i < nrows; i++){
    for(int j = 0; j < ncols; j++){
      printf("%c ", matrix[i][j]);
    }
    printf("\n");
  }
}

int* commonLetter(char* word1, char* word2){
  int* indices = malloc(sizeof(int)*2);
  for(int i = 0; i < strlen(word1); i++){
    for(int j = 0; j < strlen(word2); j++){
      if(word1[i] == word2[j]){
        indices[0] = i;
        indices[1] = j;
        return indices;
      }
    }
  }
  free(indices);
  return NULL;
}

void makeMatrix(char* word1, char* word2){

  int* commonLetterIndices = commonLetter(word1, word2);
  if (commonLetterIndices == NULL){
    printf("No common letter!");
    exit(0);
  }

  char** crossword = malloc(sizeof(char*)*strlen(word1));
  char free_char = '.';
  for(int i = 0; i < strlen(word1); i++){
    crossword[i] = malloc(sizeof(char)*strlen(word2));
    for(int j = 0; j < strlen(word2); j++) crossword[i][j] = free_char;
  }

  for(int i = 0; i < strlen(word1); i++){
    crossword[i][commonLetterIndices[1]] = word1[i];
  }

  for(int i = 0; i <strlen(word2); i++){
    crossword[commonLetterIndices[0]][i] = word2[i];
  }

  printMatrix(crossword, strlen(word1), strlen(word2));
  
  for(int i = 0; i < strlen(word1); i++){
    free(crossword[i]);
  }

  free(crossword);
  free(commonLetterIndices);

}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Usage: %s <word1> <word2>\n", argv[0]);
    exit(0);
  }

  makeMatrix(argv[1], argv[2]);
  return 0;
}
