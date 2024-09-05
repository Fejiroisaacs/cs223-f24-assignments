/***************************************************
 * mathgame.c
 * Author: Oghenefejiro Anigboro 
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
	int totalGames, x, y, userSol, sol;
	int correct = 0;

	printf("Welcome to my math game. How many games would you like to play? ");
	scanf(" %d", &totalGames);

	for (int i = 0; i < totalGames; i++){
		x = rand() % 10 + 1;
		y = rand() % 10 + 1;
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
	printf("Game over, you got %d/%d correctly!", correct, totalGames);
	return 0;
}
