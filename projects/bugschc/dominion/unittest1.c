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

#define FUNCTION	"discardCard()"

int main() {
	int testCount = 0;
	int totalTests = 0;
	int testPassed = 0;
	int totalPassed = 0;

	int index;
	int handpos = 0, trashFlag = 0;

	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	//set a cards in hand to be specific cards 
	G.hand[thisPlayer][0] = copper;
	G.hand[thisPlayer][1] = silver;	
	G.hand[thisPlayer][2] = gold;	
	G.hand[thisPlayer][3] = estate;
	G.hand[thisPlayer][4] = duchy;	

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf(BOLD_YELLOW "----------------- Testing Function: %s ----------------" COLOR_RESET "\n", FUNCTION);

	// ----------- TEST 1: Card is no longer in hand --------------
	printf("TEST 1: Card is no longer in hand \n");

	//print the list of starting cards in hand
	printf(YELLOW "Starting cards:\t");
	for (index = 0; index < G.handCount[thisPlayer]; index++) {
		printf("%s, ", cardName(G.hand[thisPlayer][index], &G));
	}
		//get the name of the card we're discarding
		char* discardedCard = cardName(testG.hand[thisPlayer][handpos], &testG);

		//get enum value of discarded card
		int discardValue = handCard(handpos, &testG);

		printf(COLOR_RESET "\nDiscarding %s...\n", discardedCard);

		//call discard function
		discardCard(handpos, thisPlayer, &testG, trashFlag);

		int foundCard = 0;

		testCount++;
		//check discarded card is no longer in hand
		for (index = 0; index < testG.handCount[thisPlayer]; index++) {
			if (handCard(index, &testG) == discardValue) { foundCard = 1; }
		}

		if (foundCard == 0) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\tending cards:\t");
		for (index = 0; index < testG.handCount[thisPlayer]; index++) {

			printf("%s, ", cardName(testG.hand[thisPlayer][index], &testG));
		}

		//print current testing results
		printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
		totalTests += testCount;
		totalPassed += testPassed;

		// ----------- TEST 2: discard count --------------
		printf("\nTEST 2: Discarded card count \n");

		//reset counters
		testCount = 0;
		testPassed = 0;

		testCount++;
		if (testG.discardCount[thisPlayer] == 1) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\tdiscard count = %d, expected = 1", testG.discardCount[thisPlayer]);

		//print current testing results
		printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
		totalTests += testCount;
		totalPassed += testPassed;

		// ----------- TEST 3: trash card --------------
		printf("\nTEST 3: Trash a card\n");

		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));

		//reset counters
		testCount = 0;
		testPassed = 0;

		//set trash flag
		trashFlag = 1;

		//call discard function
		discardCard(handpos, thisPlayer, &testG, trashFlag);

		testCount++;
		if (testG.discardCount[thisPlayer] - testG.playedCardCount == 1) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\ttrashed card count = %d, expected = 1", testG.discardCount[thisPlayer] - testG.playedCardCount);

		//print current testing results
		printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
		totalTests += testCount;
		totalPassed += testPassed;

		// ----------- TEST 4: trash card not played--------------
		printf("\nTEST 4: Trashed card was not played\n");

		//reset counters
		testCount = 0;
		testPassed = 0;

		testCount++;
		if (testG.playedCardCount == 0) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\tplayed card count = %d, expected = 0", testG.playedCardCount);

		//print current testing results
		printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
		totalTests += testCount;
		totalPassed += testPassed;

		// ----------- TEST 5: discard last card in hand--------------
		printf("\nTEST 5: Discard last card in hand\n");

		//reset counters
		testCount = 0;
		testPassed = 0;

		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));

		handpos = 4;

		//print the list of starting cards in hand
		printf(YELLOW "Starting cards:\t");
		for (index = 0; index < G.handCount[thisPlayer]; index++) {
			printf("%s", cardName(G.hand[thisPlayer][index], &G));
		}
		//get the name of the card we're discarding
		discardedCard = cardName(testG.hand[thisPlayer][handpos], &testG);

		//get enum value of discarded card
		discardValue = handCard(handpos, &testG);

		printf(COLOR_RESET "\nDiscarding %s...\n", discardedCard);

		//call discard function
		discardCard(handpos, thisPlayer, &testG, trashFlag);

		foundCard = 0;

		testCount++;
		//check discarded card is no longer in hand
		for (index = 0; index < testG.handCount[thisPlayer]; index++) {
			if (handCard(index, &testG) == discardValue) { foundCard = 1; }
		}

		if (foundCard == 0) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\tending cards:\t");
		for (index = 0; index < testG.handCount[thisPlayer]; index++) {

			printf("%s, ", cardName(testG.hand[thisPlayer][index], &testG));
		}

		//print current testing results
		printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
		totalTests += testCount;
		totalPassed += testPassed;


		// ----------- TEST 6: discard only card in hand--------------
		printf("\nTEST 6: Discard only card in hand\n");

		//reset counters
		testCount = 0;
		testPassed = 0;

		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));

		handpos = 0;
		
		//discard the first 4 cards
		for (index = 0; index < 4; index++) {
			discardCard(handpos, thisPlayer, &testG, trashFlag);
		}

		//print the list of starting cards in hand
		printf(YELLOW "Starting cards:\t");
		for (index = 0; index < testG.handCount[thisPlayer]; index++) {
			printf("%s, ", cardName(testG.hand[thisPlayer][index], &testG));
		}
		//get the name of the card we're discarding
		discardedCard = cardName(testG.hand[thisPlayer][handpos], &testG);

		//get enum value of discarded card
		discardValue = handCard(handpos, &testG);

		printf(COLOR_RESET "\nDiscarding %s...\n", discardedCard);

		//call discard function
		discardCard(handpos, thisPlayer, &testG, trashFlag);

		foundCard = 0;

		testCount++;
		//check discarded card is no longer in hand
		for (index = 0; index < testG.handCount[thisPlayer]; index++) {
			if (handCard(index, &testG) == discardValue) { foundCard = 1; }
		}

		if (foundCard == 0) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\tending cards:\t");
		for (index = 0; index < testG.handCount[thisPlayer]; index++) {

			printf("%s, ", cardName(testG.hand[thisPlayer][index], &testG));
		}

		//test hand count
		testCount++;
		if (testG.handCount[thisPlayer] == 0) {
			printf(GREEN);
			testPassed++;
		}
		else {
			printf(RED);
		}
		printf("\n\tcards in hand = %d, expected = 0", testG.handCount[thisPlayer]);

		//print current testing results
		printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
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

		printf("\n >> >> > %s: Testing complete %s << << < " COLOR_RESET "\n", testResult, FUNCTION);
	return 0;
}