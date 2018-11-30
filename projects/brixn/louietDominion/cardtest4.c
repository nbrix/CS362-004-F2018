/*****************************************************************************
Name: Nikolas Brix
Date: 10/28/2018
Descripton: Unit test for Village card.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./rngs.h"

#define NUM_PLAYERS 4

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

int testVillage(struct gameState *state, struct gameState *initState)
{
  int testPassed = 0;
  int player = 0;
  int placeHolder = 0;
  int *bonus = &placeHolder;
  int initHandCount = state->handCount[player];
  int handPos = 0;

  state->hand[player][0] = village;
  cardEffect(village, placeHolder, placeHolder, placeHolder, state, handPos, bonus);

  // test state action increases by 2
  printf("--------- START OF CARD TEST 4 ---------\n");
  testPassed += Assert(state->numActions == (initState->numActions + 2), "Number of Actions increased by 2");

  // test card is discarded
  int i = 0;
  bool isCardDiscarded = true;
  for (i = 0; i < state->handCount[player]; i++) {
    if (state->hand[player][i] == village) {
      isCardDiscarded = false;
    }
  }
  testPassed += Assert(isCardDiscarded, "Card is discarded");

  // test player draws 1 card
  int currentHandCount = state->handCount[player];
  testPassed += Assert((initHandCount == currentHandCount) && isCardDiscarded, "Player draws one card");

  // test supply piles stay the same
  i = 0;
  bool isSameCount = true;
  for (i = 0; i <= treasure_map; i++) {
    if (state->supplyCount[i] != initState->supplyCount[i]) {
      isSameCount = false;
    }
  }
  testPassed += Assert(isSameCount, "Supply piles stay the same");

  // test other player's hand and deck stays the same
  testPassed += Assert((initState->handCount[1]) == state->handCount[1], "Player 2 hand stayed the same");
  testPassed += Assert((initState->handCount[2]) == state->handCount[2], "Player 3 hand stayed the same");
  testPassed += Assert((initState->handCount[3]) == state->handCount[3], "Player 4 hand stayed the same");

  testPassed += Assert((initState->deckCount[1]) == state->deckCount[1], "Player 2 deck stayed the same");
  testPassed += Assert((initState->deckCount[2]) == state->deckCount[2], "Player 3 deck stayed the same");
  testPassed += Assert((initState->deckCount[3]) == state->deckCount[3], "Player 4 deck stayed the same");

  return testPassed;
}

int main()
{
  struct gameState G, initialG;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
  int numPlayers = NUM_PLAYERS;

  srand(time(0));

  initializeGame(numPlayers, k, rand() % 1000, &G);
  memcpy(&initialG, &G, sizeof(struct gameState));
  int testResults = testVillage(&G, &initialG);

  if (testResults > 0)
    printf("TEST FAILED\n\n");
  else
    printf("TEST SUCCESSFULLY COMPLETED\n\n");

  return 0;
}