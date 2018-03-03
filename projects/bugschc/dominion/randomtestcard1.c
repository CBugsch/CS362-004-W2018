
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

#define TESTCARD	"Village"

int seed;

int testCardEffect(struct gameState* post, int handpos, int results[5]) {
	int failedTests = 0;
	
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));
	
	playVillage(post, 0, handpos);

	//test # of cards in player's hand count
	if (post->handCount[0] != pre.handCount[0]) {
		results[0]++;
	}

	//test # of cards in players deck
	if (post->deckCount[0] != pre.deckCount[0] - 1) {
		results[1]++;
		//printf("FAILED ON SEED %d\n", seed);	
	}

	//test # of cards in players discard deck
	if (post->discardCount[0] != pre.discardCount[0] + 1) {
		results[2]++;
	}

	//test last card in discard deck is village card
	if (post->discard[0][post->discardCount[0] - 1] != village) {
		results[3]++;
	}

	//test # of actions increased by least 2
	if (post->numActions != pre.numActions + 2) {
		results[4]++;
	}

	

	return failedTests;

}

void printGameState(struct gameState* game) {
	printf("Hand Count %d\n", game->handCount[0]);
	printf("Deck Count %d\n", game->deckCount[0]);
	printf("Discard Count %d\n", game->discardCount[0]);
	printf("Played Count %d\n", game->playedCardCount);
}

int main() {
	int results[5] = { 0, 0, 0, 0, 0 };
	int i, j;
	int handpos;

	struct gameState game;
	
	for (i = 0; i < 10000; i++) {
		seed = i;
		srand(seed);
		
		for (j = 0; j < sizeof(struct gameState); j++) {
			((char*)&game)[j] = rand() % 256 + 1;
		}

		//RANDOMIZE THE PLAYERS DECK
		game.deckCount[0] = rand() % MAX_DECK;
		for (j = 0; j < game.deckCount[0]; j++) {
			game.deck[0][j] = rand() % treasure_map + 1;
		}
	
		//RANDOMIZE THE PLAYERS HAND (SET FIRST CARD TO VILLAGE)
		game.handCount[0] = rand() % MAX_DECK + 1;
		for (j = 0; j < game.handCount[0]; j++) {
			game.hand[0][j] = rand() % treasure_map + 1;
		}
		handpos = rand() % game.handCount[0];
		game.hand[0][handpos] = village;
		
		//RANDOMIZE THE PLAYERS DISCARD DECK
		game.discardCount[0] = rand() % MAX_DECK;
		for (j = 0; j < game.discardCount[0]; j++) {
			game.discard[0][j] = rand() % treasure_map + 1;
		}

		//RANDOMIZE THE PLAYED DECK
		game.playedCardCount = rand() % MAX_DECK;
		for (j = 0; j < game.playedCardCount; j++) {
			game.playedCards[j] = rand() % treasure_map + 1;
		}
	
		//RANDOMINZE THE PLAYERS ACTION COUNT
		game.numActions = rand() % 5;
		

		//printGameState(&game);

		//CALL THE TEST
		testCardEffect(&game, handpos, results);

		//printGameState(&game);
	}

	printf("Test #1: Number of cards in hand: Failed %d/10000 times\n", results[0]);
	printf("Test #2: Number of cards in deck: Failed %d/10000 times\n", results[1]);
	printf("Test #3: Number of cards in discard deck: Failed %d/10000 times\n", results[2]);
	printf("Test #4: Last card in discard deck is card played: Failed %d/10000 times\n", results[3]);
	printf("Test #5: Number of actions: Failed %d/10000 times\n", results[4]);



	return 0;
}