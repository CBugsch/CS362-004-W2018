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

#define FUNCTION	"isGameOver()"

char* resultString(int num) {
	if (num == 0) { return "Game not over"; }
	else { return "Game over"; }
}

int main() {
	int testCount = 0;
	int totalTests = 0;
	int testPassed = 0;
	int totalPassed = 0;

	int result;


	int seed = 1000;
	int numPlayers = 2;
	
	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf(BOLD_YELLOW "----------------- Testing Function: %s ----------------" COLOR_RESET "\n", FUNCTION);

	// ----------- TEST 1: No province cards left --------------
	printf("TEST 1: No province cards left \n");

	//set province deck to 0
	testG.supplyCount[province] = 0;

	//call function
	result = isGameOver(&testG);

	testCount++;
	if (result == 1) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tresult = %s, expected = Game over", resultString(result));


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 2: More then 3 kingdom decks empty --------------
	printf("\nTEST 2: More then 3 kingdom decks empty \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//set 3 kingdom decks to 0
	testG.supplyCount[village] = 0;
	testG.supplyCount[mine] = 0;
	testG.supplyCount[smithy] = 0;

	//call function
	result = isGameOver(&testG);

	testCount++;
	if (result == 1) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tresult = %s, expected = Game over", resultString(result));


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 3: All coin decks empty --------------
	printf("\nTEST 3: All coin decks empty \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//set 3 coin decks to 0
	testG.supplyCount[copper] = 0;
	testG.supplyCount[silver] = 0;
	testG.supplyCount[gold] = 0;

	//call function
	result = isGameOver(&testG);

	testCount++;
	if (result == 1) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tresult = %s, expected = Game over", resultString(result));


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 4: Random decks empty--------------
	printf("\nTEST 4: Random decks empty \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//set 3 decks to 0
	testG.supplyCount[curse] = 0;
	testG.supplyCount[duchy] = 0;
	testG.supplyCount[estate] = 0;

	//call function
	result = isGameOver(&testG);

	testCount++;
	if (result == 1) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tresult = %s, expected = Game over", resultString(result));


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;
	

	// ----------- TEST 5: No cards drawn --------------
	printf("\nTEST 5: No cards drawn \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	//call function
	result = isGameOver(&testG);

	testCount++;
	if (result == 0) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tresult = %s, expected = Game not over", resultString(result));


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