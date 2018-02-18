
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <math.h>

#define RED		"\x1b[31m"
#define YELLOW		"\x1b[33m"
#define BOLD_YELLOW	"\033[01;33m"
#define GREEN		"\x1b[32m"
#define COLOR_RESET	"\033[0m"

#define TESTCARD	"Adventurer"



int testCardEffect(struct gameState* post,int results[3]  ) {


	int failedTests = 0;
	
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	playAdventurer(post, 0);

	
	//test # of cards in player's hand count
	if (post->handCount[0] != pre.handCount[0] + 1) {
		results[0]++;
	}
	

	//test # of coins increased by at least 2
	updateCoins(0, post, 0);
	updateCoins(0, &pre, 0);
	if (post->coins <= pre.coins + 2) {
		results[1]++;
	}
		
	//check the last 2 cards in the hand are treasure cards
	if ((post->hand[0][post->handCount[0] - 1] <= copper || post->hand[0][post->handCount[0] - 1] >= gold)
		&& (post->hand[0][post->handCount[0] - 2] <= copper || post->hand[0][post->handCount[0] - 2] >= gold)) {
		results[2]++;
	}


	return failedTests;

}

int main() {
	int seed;
	int results[3] = { 0, 0, 0 };
	int i, j;
	
	struct gameState game;


	for (i = 0; i < 10000; i++) {
		seed = i;
		srand(seed);
		
		for (j = 0; j < sizeof(struct gameState); j++) {
			((char*)&game)[j] = rand() % 257;
		}

		//RANDOMIZE THE PLAYERS DECK
		game.deckCount[0] = rand() % MAX_DECK;
		for (j = 0; j < game.deckCount[0]; j++) {
			game.deck[0][j] = rand() % treasure_map + 1;
		}
		
		//RANDOMIZE THE PLAYERS HAND (SET FIRST CARD TO ADVENTURER)
		game.handCount[0] = rand() % MAX_DECK;
		for (j = 0; j < game.handCount[0]; j++) {
			game.hand[0][j] = rand() % treasure_map + 1;
		}
		game.hand[0][0] = adventurer;

		//RANDOMIZE THE PLAYERS DISCARD DECK
		game.discardCount[0] = rand() % MAX_DECK;
		for (j = 0; j < game.discardCount[0]; j++) {
			game.discard[0][j] = rand() % treasure_map + 1;
		}

		testCardEffect(&game, results);
		
		
	}

printf("Test #1: Number of cards in hand: Failed %d/10000 times\n", results[0]);
printf("Test #2: Number of coins increased: Failed %d/10000 times\n", results[1]);
printf("Test #3: Last two cards in hand are treasure cards: Failed %d/10000 times\n", results[2]);

	

	return 0;
}
