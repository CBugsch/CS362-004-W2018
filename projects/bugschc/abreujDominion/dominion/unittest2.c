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

#define FUNCTION	"updateCoins()"

int main() {
	int testCount = 0;
	int totalTests = 0;
	int testPassed = 0;
	int totalPassed = 0;

	int index;

	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy, council_room };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	//set a cards in hand to be copper cards 
	for (index = 0; index < 5; index++) {
		G.hand[thisPlayer][index] = silver;
	}

	G.coins = 10;

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));

	printf(BOLD_YELLOW "----------------- Testing Function: %s ----------------" COLOR_RESET "\n", FUNCTION);

	// ----------- TEST 1: Coin count goes down --------------
	printf("TEST 1: Coin count goes down \n");

	testCount++;
	printf("Changing silver to copper...\n");

	testG.hand[thisPlayer][0] = copper;

	//call function
	updateCoins(thisPlayer, &testG, 0);

	if (testG.coins == G.coins - 1) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tcoins = %d, expected = %d", testG.coins, G.coins - 1);
	

	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 2: Coin count goes up --------------
	printf("\nTEST 2: Coin count goes up \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	testCount++;
	printf("Changing copper back to silver...\n");

	testG.hand[thisPlayer][0] = silver;

	//call function
	updateCoins(thisPlayer, &testG, 0);

	if (testG.coins == G.coins) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tcoins = %d, expected = %d", testG.coins, G.coins);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 3: Bonus gets applied --------------
	printf("\nTEST 3: Bonus get applied \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	testCount++;
	printf("Applying a +2 bonus...\n");

	//call function
	updateCoins(thisPlayer, &testG, 2);

	if (testG.coins == G.coins + 2) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tcoins = %d, expected = %d", testG.coins, G.coins + 2);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 4: Correct value of copper --------------
	printf("\nTEST 4: Correct value of copper \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	testCount++;
	
	//set hand to 1 copper
	testG.hand[thisPlayer][0] = copper;
	testG.handCount[thisPlayer] = 1;

	//call function
	updateCoins(thisPlayer, &testG, 0);

	if (testG.coins == 1) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tcoins = %d, expected = 1", testG.coins);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 5: Correct value of silver --------------
	printf("\nTEST 5: Correct value of silver \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	testCount++;

	//set hand to 1 silver
	testG.hand[thisPlayer][0] = silver;

	//call function
	updateCoins(thisPlayer, &testG, 0);

	if (testG.coins == 2) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tcoins = %d, expected = 2", testG.coins);


	//print current testing results
	printf(BOLD_YELLOW "\nTest Passed: %d/%d" COLOR_RESET "\n", testPassed, testCount);
	totalTests += testCount;
	totalPassed += testPassed;

	// ----------- TEST 6: Correct value of gold --------------
	printf("\nTEST 6: Correct value of gold \n");

	//reset counters
	testCount = 0;
	testPassed = 0;

	testCount++;

	//set hand to 1 gold
	testG.hand[thisPlayer][0] = gold;

	//call function
	updateCoins(thisPlayer, &testG, 0);

	if (testG.coins == 3) {
		printf(GREEN);
		testPassed++;
	}
	else {
		printf(RED);
	}
	printf("\tcoins = %d, expected = 3", testG.coins);


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