
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

#define TESTCARD	"Village"

int main() {
	int testCount = 0;
	int totalTests = 0;
	int testPassed = 0;
	int totalPassed = 0;

	int newCards = 1;
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

	//set a card in the players hand to be a village
	G.hand[thisPlayer][0] = village;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//call village action function
	cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

	printf(BOLD_YELLOW "----------------- Testing Card: %s ----------------" COLOR_RESET "\n", TESTCARD);

	// ----------- TEST 1: Player receives exactly 1 cards --------------
	printf("TEST 1: Current player receives exactly 1 cards and discards village card\n");

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

	//test that we discarded the correct card
	testCount++;
	if (testG.hand[thisPlayer][0] != village) {
		printf(GREEN);
		testPassed++;
	}
	else { printf(RED); }
	printf("\tstarting cards:\t");
	for (index = 0; index < G.handCount[thisPlayer]; index++) {
		printf("%s, ", cardName(G.hand[thisPlayer][index], &G));
	}

	printf("\n\tending cards:\t");
	for (index = 0; index < testG.handCount[thisPlayer]; index++) {
		printf("%s, ", cardName(testG.hand[thisPlayer][index], &testG));
	}

	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 2: player has 2 additional actions --------------
	printf("TEST 2: Player has exactly 2 additional actions\n");
	
	//reset counters
	testCount = 0;
	testPassed = 0;

	testCount++;
	if (testG.numActions == G.numActions + 1) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	
	printf("\tNumber of actions left = %d, expected = %d" COLOR_RESET "\n", testG.numActions, G.numActions + 1);

	//print current testing results
	printf(BOLD_YELLOW "Test Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;
	

	
	// ----------- TEST 3: other players state has not changed --------------
	printf("\nTEST 3: No state change to other players\n");

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

	// ----------- TEST 4: No state change to card piles --------------
	printf("\nTEST 4: No state change to supply decks\n");

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

	printf("\n >> >> > %s: Testing complete %s << << < " COLOR_RESET "\n\n", testResult, TESTCARD);



	return 0;
}


