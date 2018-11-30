/*****************************************************************************
Name: Nikolas Brix
Date: 10/28/2018
Descripton: Unit test for Smithy card.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./rngs.h"

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

int testSmithy(struct gameState *state)
{
  int testPassed = 0;
  struct gameState testState;
  int player = 0;
  int player2 = 1;
  int placeHolder = 0;
  int *bonus = &placeHolder;
  int initHandCount = state->handCount[player];
  int handPos = 0;

  memcpy(&testState, state, sizeof(struct gameState));
  state->hand[player][0] = smithy;
  cardEffect(smithy, placeHolder, placeHolder, placeHolder, state, handPos, bonus);

  // test if other hands are effected
  printf("--------- START OF CARD TEST 2 ---------\n");
  testPassed += Assert(state->handCount[player2] == testState.handCount[player2], "Player 2 hand has not changed");

  // test victory card piles and kingdom card piles stay the same
  int i = 0;
  int isSameCount = true;
  for (i = 0; i <= treasure_map; i++) {
    if (state->supplyCount[i] != testState.supplyCount[i]) {
      isSameCount = false;
    }
  }
  testPassed += Assert(isSameCount, "Supply piles stay the same");

  state->hand[player][0] = smithy;
  cardEffect(smithy, placeHolder, placeHolder, placeHolder, state, handPos, bonus);

  // test number of cards in hand with deck containing at least 3 cards
  int currentHandCount = state->handCount[player];
  testPassed += Assert((initHandCount + 2) == currentHandCount, "Deck contains at least 3 cards");

  // reset
  state->handCount[player] = 5;
  state->deckCount[player] = 2;
  state->discardCount[player] = 0;
  state->hand[player][0] = smithy;

  cardEffect(smithy, placeHolder, placeHolder, placeHolder, state, handPos, bonus);

  // test with deck containing 2 cards
  currentHandCount = state->handCount[player];
  testPassed += Assert((initHandCount + 1) == currentHandCount, "Deck contains 2 cards");

  // reset
  state->handCount[player] = 5;
  state->deckCount[player] = 1;
  state->discardCount[player] = 0;
  state->hand[player][0] = smithy;

  cardEffect(smithy, placeHolder, placeHolder, placeHolder, state, handPos, bonus);

  // test with deck containing one card
  currentHandCount = state->handCount[player];
  testPassed += Assert((initHandCount) == currentHandCount, "Deck contains 1 card");

  // reset
  state->handCount[player] = 5;
  state->deckCount[player] = 0;
  state->discardCount[player] = 0;
  state->hand[player][0] = smithy;

  cardEffect(smithy, placeHolder, placeHolder, placeHolder, state, handPos, bonus);

  // test with empty deck
  currentHandCount = state->handCount[player];
  testPassed += Assert((initHandCount - 1) == currentHandCount, "Empty deck");

  return testPassed;
}

int main()
{
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
  int numPlayers = MIN_PLAYERS;

  srand(time(0));

  initializeGame(numPlayers, k, rand() % 1000, &G);
  int testResults = testSmithy(&G);

  if (testResults > 0)
    printf("TEST FAILED\n\n");
  else
    printf("TEST SUCCESSFULLY COMPLETED\n\n");

  return 0;
}