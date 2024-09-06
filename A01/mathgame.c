/***************************************************
 * mathgame.c
 * This program implements the math game which asks the users
 * a series of addition questions which they specify at the start
 * @author: Oghenefejiro Anigboro
 * @version: September 6th, 2020 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
	
int main() {
  srand(time(NULL));
  int totalGames, x, y, userSol, sol, i;
	int correct = 0;

	printf("Welcome to my math game. How many games would you like to play? ");
	scanf(" %d", &totalGames);

	for (i = 0; i < totalGames; i++){
		x = rand() % 9 + 1; //gets a random number from 1-9
		y = rand() % 9 + 1; 
		sol = x + y;

		printf("\n%d + %d = ? ", x, y);
		scanf(" %d", &userSol);

		if (userSol == sol){
			printf("Correct, well done!\n");
			correct++;
		} else{
			printf("Incorrect :), the correct answer is %d\n", sol);
		}

	}

	printf("Game over, you got %d/%d correctly.\n", correct, totalGames);
	return 0;

}
