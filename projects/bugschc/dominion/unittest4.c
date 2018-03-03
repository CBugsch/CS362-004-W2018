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

#define FUNCTION	"scoreFor()"

int main() {
	int testCount = 0;
	int totalTests = 0;
	int testPassed = 0;
	int totalPassed = 0;

	int score;
	int index = 0;

	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;

	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, great_hall, gardens };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf(BOLD_YELLOW "----------------- Testing Function: %s ----------------" COLOR_RESET "\n", FUNCTION);

	// ----------- TEST 1: Score of curse --------------
	printf("TEST 1: Score of curse \n");

	testG.hand[thisPlayer][0] = curse;
	testG.handCount[thisPlayer] = 1;

	//call function
	score = scoreFor(thisPlayer, &testG);

	testCount++;
	if (score == -1) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tscore = %d, expected = -1", score);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 2: Score of estate --------------
	printf("\nTEST 2: Score of estate \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	testG.hand[thisPlayer][0] = estate;

	//call function
	score = scoreFor(thisPlayer, &testG);

	testCount++;
	if (score == 1) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tscore = %d, expected = 1", score);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 3: Score of duchy --------------
	printf("\nTEST 3: Score of duchy \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	testG.hand[thisPlayer][0] = duchy;

	//call function
	score = scoreFor(thisPlayer, &testG);

	testCount++;
	if (score == 3) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tscore = %d, expected = 3", score);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 4: Score of province --------------
	printf("\nTEST 4: Score of province \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	testG.hand[thisPlayer][0] = province;

	//call function
	score = scoreFor(thisPlayer, &testG);

	testCount++;
	if (score == 6) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tscore = %d, expected = 6", score);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 5: Score of great hall --------------
	printf("\nTEST 5: Score of great hall \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	testG.hand[thisPlayer][0] = great_hall;

	//call function
	score = scoreFor(thisPlayer, &testG);

	testCount++;
	if (score == 1) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tscore = %d, expected = 1", score);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 6: Score of gardens with 9 cards --------------
	printf("\nTEST 6: Score of gardens with 9 cards \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	for (index = 0; index < 8; index++) {
		testG.hand[thisPlayer][index] = estate;
	}

	testG.hand[thisPlayer][8] = gardens;
	testG.handCount[thisPlayer] = 9;

	//call function
	score = scoreFor(thisPlayer, &testG);

	testCount++;
	if (score == 8) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tscore = %d, expected = 8", score);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 7: Score of gardens with 10 cards --------------
	printf("\nTEST 7: Score of gardens with 10 cards \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	testG.hand[thisPlayer][9] = estate;
	testG.deckCount[thisPlayer] = 0;
	testG.handCount[thisPlayer] = 10;
	testG.discardCount[thisPlayer] = 0;

	//call function
	score = scoreFor(thisPlayer, &testG);

	testCount++;
	if (score == 10) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tscore = %d, expected = 10", score);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 8: Score from cards in hand --------------
	printf("\nTEST 8: Score from cards in hand \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	for (index = 0; index < 5; index++) {
		testG.hand[thisPlayer][index] = estate;
	}

	testG.handCount[thisPlayer] = 5;

	//call function
	score = scoreFor(thisPlayer, &testG);

	testCount++;
	if (score == 5) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tscore = %d, expected = 5", score);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 9: Score from cards in deck --------------
	printf("\nTEST 9: Score from cards in deck \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	for (index = 0; index < 5; index++) {
		testG.deck[thisPlayer][index] = estate;
	}

	testG.deckCount[thisPlayer] = 5;
	testG.handCount[thisPlayer] = 0;
	testG.discardCount[thisPlayer] = 0;

	//call function
	score = scoreFor(thisPlayer, &testG);

	testCount++;
	if (score == 5) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tscore = %d, expected = 5", score);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 10: Score from cards discarded --------------
	printf("\nTEST 10: Score from cards discarded \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	for (index = 0; index < 5; index++) {
		testG.discard[thisPlayer][index] = estate;
	}

	testG.deckCount[thisPlayer] = 0;
	testG.handCount[thisPlayer] = 0;
	testG.discardCount[thisPlayer] = 5;

	//call function
	score = scoreFor(thisPlayer, &testG);

	testCount++;
	if (score == 5) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tscore = %d, expected = 5", score);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 11: Score from cards trashed --------------
	printf("\nTEST 11: Score from cards trashed \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//give the player an estate card
	testG.hand[thisPlayer][0] = estate;

	//trash the card
	discardCard(0, thisPlayer, &testG, 1);
	

	//call function
	score = scoreFor(thisPlayer, &testG);

	testCount++;
	if (score == 0) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tscore = %d, expected = 0", score);


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

	printf("\n >> >> > %s: Testing complete %s << << < " COLOR_RESET "\n\n", testResult, FUNCTION);
	return 0;
}