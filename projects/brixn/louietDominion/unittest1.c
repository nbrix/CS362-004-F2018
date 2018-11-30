/*****************************************************************************
Name: Nikolas Brix
Date: 10/28/2018
Descripton: Unit test for drawCard function.
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

int testDrawCard(struct gameState *state, struct gameState *initState)
{
  int player = 0; // only one player will be tested
  int initDiscardCount = 10;  //number of cards in discard pile
  int initHandCount = state->handCount[player]; // number of cards in player's hand
  int testPassed = 0; // flag that indicates if assertion passed

  // test when deck is empty by setting deck to zero and number of
  // discarded cards to 10
  state->deckCount[player] = 0;
  state->discardCount[player] = initDiscardCount;

  drawCard(player, state);

  int deckCount = state->deckCount[player];
  printf("--------- START OF UNIT TEST 1 ---------\n");
  printf("Start from empty deck:\n");
  testPassed += Assert(deckCount == (initDiscardCount - 1), "Shuffle discard pile");
  testPassed += Assert(initHandCount == (state->handCount[player] - 1), "Number of cards in hand increases");

  // test when deck is not empty
  int initDeckCount = state->deckCount[player];
  initHandCount = state->handCount[player];
  drawCard(player, state);

  printf("Start from non-empty deck:\n");
  testPassed += Assert(state->deckCount[player] == (initDeckCount - 1), "Deck decreases by 1");
  testPassed += Assert(initHandCount == (state->handCount[player] - 1), "Number of cards in hand increases");

  // test supply piles stay the same
  int i = 0;
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
  struct gameState G, initG;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
  int numPlayers = NUM_PLAYERS;

  srand(time(0));

  initializeGame(numPlayers, k, rand() % 1000, &G);
  memcpy(&initG, &G, sizeof(struct gameState));
  int testResults = testDrawCard(&G, &initG);

  if (testResults > 0)
    printf("TEST FAILED\n\n");
  else
    printf("TEST SUCCESSFULLY COMPLETED\n\n");

  return 0;
}