
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define RED		"\x1b[31m"
#define YELLOW		"\x1b[33m"
#define BOLD_YELLOW	"\033[01;33m"
#define GREEN		"\x1b[32m"
#define COLOR_RESET	"\033[0m"

#define TESTCARD	"Salvager"

int main() {
	int testCount = 0;
	int totalTests = 0;
	int testPassed = 0;
	int totalPassed = 0;

	int discarded = 2;
	int cost = 1;

	int index;
	int handpos = 0, choice1 = 1, choice2 = 0, choice3 = 0, bonus = 0;

	int seed = 1000;
	int numPlayers = 3;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, salvager };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	//set a cards in hand to be specific cards for testing cost
	G.hand[thisPlayer][0] = salvager;
	G.hand[thisPlayer][1] = estate;	//cost 2
	G.hand[thisPlayer][2] = village;	//cost 3
	G.hand[thisPlayer][3] = smithy;	//cost 4
	G.hand[thisPlayer][4] = minion;	//cost 5

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//call village action function
	cardEffect(salvager, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf(BOLD_YELLOW "----------------- Testing Card: %s ----------------" COLOR_RESET "\n", TESTCARD);

	// ----------- TEST 1: Player has correct # of cards --------------
	printf("TEST 1: Current player has correct number of cards in hand\n");

	//test # of cards in player's hand count
	testCount++;
	if (testG.handCount[thisPlayer] == (G.handCount[thisPlayer] - discarded)) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\thand count = %d, expected = %d" COLOR_RESET "\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] - discarded);

	

	//print current testing results
	printf(BOLD_YELLOW "Test Passed: %d/%d" COLOR_RESET "\n\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 2: player discarded correct card and gained correct coins --------------
	printf("TEST 2: Player discarded correct cards and gained correct number of coins\n");

	//reset counters
	testCount = 0;
	testPassed = 0;
	int j;

	//print the list of starting cards in hand
	printf(YELLOW "Starting cards:\t");
	for (j = 0; j < G.handCount[thisPlayer]; j++) {
		printf("%s, ", cardName(G.hand[thisPlayer][j], &G));
	}
	printf( "\nStarting coins: %d" COLOR_RESET "\n", G.coins);

	for (index = 1; index < 5; index++) {
		testCount += 2;
		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));

		//get the name of the card we're discarding
		char* discardCard = cardName(testG.hand[thisPlayer][index], &testG);
		
		//get enum value of discarded card
		int discardValue = handCard(index, &testG);
		
		printf("Discarding %s (cost = %d)...\n", discardCard, getCost(discardValue));
		
		//call salvager action function with different choices
		cardEffect(salvager, index, choice2, choice3, &testG, handpos, &bonus);
		
		int foundCard = 0;

		//check discarded cards are no longer in hand
		for (j = 0; j < testG.handCount[thisPlayer]; j++) {
			if (handCard(index, &testG) == salvager || handCard(index, &testG) == discardValue) { foundCard = 1; }
		}

		if (foundCard == 0) { 
			printf(GREEN); 
			testPassed++; 
		}
		else {
			printf(RED);
		}
		printf("\tending cards:\t");
		for (j = 0; j < testG.handCount[thisPlayer]; j++) {
			
			printf("%s, ", cardName(testG.hand[thisPlayer][j], &testG));
		}
		if (testG.coins == G.coins + index + cost) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\n\tCoins = %d, expected = %d" COLOR_RESET "\n", testG.coins, G.coins + index + cost);

	}


	//print current testing results
	printf(BOLD_YELLOW "Test Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 3: Trashed card was not played  --------------
	printf("\nTEST 3: Trashed card was properly trashed");

	//reset counters
	testCount = 0;
	testPassed = 0;

	//testing name of played cards
	testCount++;
	if (testG.playedCards[0] == salvager) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\n\tcards played = %s, expected = %s" COLOR_RESET "\n", cardName(testG.playedCards[0], &testG), "salvager");

	//testing number of played cards
	testCount++;
	if (testG.playedCardCount == G.playedCardCount + 1) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tplayed cards = %d, expected = %d" COLOR_RESET "\n", testG.playedCardCount, G.playedCardCount + 1);

	//testing number of discarded cards
	testCount++;
	if (testG.discardCount[thisPlayer] == (G.discardCount[thisPlayer] + discarded)) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tdiscard deck count = %d, expected = %d" COLOR_RESET "\n", testG.discardCount[thisPlayer], G.discardCount[thisPlayer] + discarded);


	

	//print current testing results
	printf(BOLD_YELLOW "Test Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 4: other players state has not changed --------------
	printf("\nTEST 4: No state change to other players\n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	//loop through all players, start at 1 because current player is #0
	for (index = 1; index < numPlayers; index++) {
		printf("Player %d:\n", index);

		//hand count
		testCount++;
		if (testG.handCount[index] == G.handCount[index]) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\thand count = %d, expected = %d" COLOR_RESET "\n", testG.handCount[index], G.handCount[index]);

		//deck count
		testCount++;
		if (testG.deckCount[index] == G.deckCount[index]) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\tdeck count = %d, expected = %d" COLOR_RESET "\n", testG.deckCount[index], G.deckCount[index]);

		//discarded deck count
		testCount++;
		if (testG.discardCount[thisPlayer] == G.discardCount[thisPlayer]) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\tdiscard deck count = %d, expected = %d" COLOR_RESET "\n", testG.discardCount[index], G.discardCount[index]);

		//players current score
		testCount++;

		if (scoreFor(index, &testG) == scoreFor(index, &G)) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\tcurrent score = %d, expected = %d" COLOR_RESET "\n", scoreFor(index, &testG), scoreFor(index, &G));
	}

	//print current testing results
	printf(BOLD_YELLOW "Test Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 5: No state change to card piles --------------
	printf("\nTEST 5: No state change to supply decks\n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	//loop through all cards
	for (index = curse; index <= sea_hag; index++) {
		if (G.supplyCount[index] != -1) { //check only cards being played
			testCount++;
			if (testG.supplyCount[index] == G.supplyCount[index]) {
				printf(GREEN);
				testPassed++;
			}
			else {
				printf(RED);
			}
			printf("\tNumber of %s cards = %d, expected = %d" COLOR_RESET "\n", cardName(index, &G), testG.supplyCount[index], G.supplyCount[index]);
		}
	}
	//print current testing results
	printf(BOLD_YELLOW "Test Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;


	// ----------------- TEST RESULTS -------------------------------
	printf(BOLD_YELLOW "\nTotal Test Passed: %d/%d" COLOR_RESET, totalPassed, totalTests);
	char* testResult;
	if (totalTests == totalPassed) {
		testResult = "SUCCESS";
		printf(GREEN);
	}
	else {
		testResult = "FAILED";
		printf(RED);
	}

	printf("\n >> >> > %s: Testing complete %s << << < " COLOR_RESET "\n", testResult, TESTCARD);



	return 0;
}


