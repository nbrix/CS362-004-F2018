/*****************************************************************************
Name: Nikolas Brix
Date: 11/12/2018
Descripton: Random test for Smithy card.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./rngs.h"

#define NUM_TESTS 100
#define MIN_PLAYERS 2

typedef int bool;
#define true 1
#define false 0

int Assert(bool condition, char* message)
{
  if (!(condition)) {
    printf("Test Failed: %s\n", message);
    return 1;
  }
  else {
    printf("Test Passed: %s\n", message);
    return 0;
  }
}

int testSmithy(struct gameState *state, struct gameState *initState, int numPlayers)
{
  int testPassed = 0;
  int placeHolder = 0;
  int *bonus = &placeHolder;
  int cards[16] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, copper, silver, gold, estate, duchy, province};

  // random variables
  int player = rand() % numPlayers;	// player to test
  int handcount = rand() % MAX_HAND;
  int deckcount = rand() % MAX_DECK;
  int discardcount = rand() % MAX_DECK;

  state->whoseTurn = player;
  state->handCount[player] = handcount;
  state->deckCount[player] = deckcount;
  state->discardCount[player] = discardcount;
  int i, j;

  // initialize deck
  for (i = 0; i < numPlayers; i++) {
  	for (j = 0; j < deckcount; j++) {
  		state->deck[i][j] = cards[rand() % 16];
  	}
  }

  // initialize hand
  for (i = 0; i < numPlayers; i++) {
  	for (j = 0; j < handcount; j++) {
  		state->hand[i][j] = cards[rand() % 16];
  	}
  }

  // count number of Smithys in hand
  int handpos = 0;
  int numSmithyInHand = 0;
  for (i = 0; i < handcount; i++) {
  	if (state->hand[player][i] == smithy) {
  		numSmithyInHand++;
      handpos = i;
  	}
  }

  // if there are no smithys in hand, put one in the first position
  if (numSmithyInHand == 0) {
    state->hand[player][0] = smithy;
    numSmithyInHand = 1;
  }

  int initHandCount = state->handCount[player];

  cardEffect(smithy, placeHolder, placeHolder, placeHolder, state, handpos, bonus);
  
  int currentHandCount = state->handCount[player];

  // test if hand increases by three
  testPassed += Assert((currentHandCount - 2) == initHandCount, "Draw three cards");

  // test supply piles stay the same
  bool isSameCount = true;
  for (i = 0; i <= treasure_map; i++) {
    if (state->supplyCount[i] != initState->supplyCount[i]) {
      isSameCount = false;
    }
  }
  testPassed += Assert(isSameCount, "Supply piles stay the same");

  // test Smithy card is discarded
  int finalSmithyCount = 0;
  for (i = 0; i < state->handCount[player]; i++) {
    if (state->hand[player][i] == smithy) {
      finalSmithyCount++;
    }
  }
  testPassed += Assert(finalSmithyCount == (numSmithyInHand - 1), "Card is discarded");
  
  return testPassed;
}

int main()
{
  srand(time(0));
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
  int playerRange = MAX_PLAYERS - MIN_PLAYERS + 1;
  int numTestsPassed = 0;
  int numTestsFailed = 0;
  int i = 0;
  struct gameState G, initialG;

  for (i = 0; i < NUM_TESTS; i++) {
  	int numPlayers = rand() % playerRange + MIN_PLAYERS;
  	initializeGame(numPlayers, k, rand() % 1000, &G);
  	memcpy(&initialG, &G, sizeof(struct gameState));
  	int testResults = testSmithy(&G, &initialG, numPlayers);

  	if (testResults > 0) {
  	  numTestsFailed++;
  	}
  	else {
  	  numTestsPassed++;
  	}
  }
    
  printf("NUMBER OF TESTS PASSED: %d\n", numTestsPassed);
  printf("NUMBER OF TESTS FAILED: %d\n\n", numTestsFailed);

  return 0;
}