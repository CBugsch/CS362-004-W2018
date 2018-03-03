
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

#define TESTCARD	"Smithy"

int main() {
	int testCount = 0;
	int totalTests = 0;
	int testPassed = 0;
	int totalPassed = 0;

	int newCards = 3;
	int discarded = 1;

	int index;
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	
	int seed = 1000;
	int numPlayers = 3;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//call smithy action function
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf(BOLD_YELLOW "----------------- Testing Card: %s ----------------" COLOR_RESET "\n", TESTCARD);

	// ----------- TEST 1: Player receives exactly 3 cards --------------
	printf("TEST 1: Current player receives exactly 3 cards\n");

	//test # of cards in player's hand count
	testCount++;
	if (testG.handCount[thisPlayer] == (G.handCount[thisPlayer] + newCards - discarded)) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\thand count = %d, expected = %d" COLOR_RESET "\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	
	//test # of cards in player's deck count
	testCount++;
	if (testG.deckCount[thisPlayer] == (G.deckCount[thisPlayer] - newCards)) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tdeck count = %d, expected = %d" COLOR_RESET "\n", testG.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards);

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
	printf(BOLD_YELLOW "Test Passed: %d/%d" COLOR_RESET "\n\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;
	

	// ----------- TEST 2: other players state has not changed --------------
	printf("TEST 2: No state change to other players\n");

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
		if (testG.discardCount[index] == G.discardCount[index]) {											
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

	// ----------- TEST 3: No state change to card piles --------------
	printf("\nTEST 3: No state change to supply decks\n");

	//reset counters
	testCount = 0;
	testPassed = 0;
	
	//loop through all cards
	for (index = curse; index <= treasure_room; index++) {
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

	printf("\n >> >> > %s: Testing complete %s << << < " COLOR_RESET "\n\n", testResult, TESTCARD);
	


	return 0;
}


