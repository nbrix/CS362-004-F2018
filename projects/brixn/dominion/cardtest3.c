/*****************************************************************************
Name: Nikolas Brix
Date: 10/28/2018
Descripton: Unit test for Council Room card.
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

int testCouncilRoom(struct gameState *state, struct gameState *initState)
{
  int testPassed = 0;
  int player = 0;
  int placeHolder = 0;
  int *bonus = &placeHolder;
  int initHandCount = state->handCount[player];
  int handPos = 0;

  state->hand[player][0] = council_room;
  cardEffect(council_room, placeHolder, placeHolder, placeHolder, state, handPos, bonus);

  // test player draws 4 cards
  int currentHandCount = state->handCount[player];
  printf("--------- START OF CARD TEST 3 ---------\n");
  testPassed += Assert((initHandCount + 3) == currentHandCount, "Player draws four cards");

  // test state increases buy by one
  testPassed += Assert((initState->numBuys + 1) == state->numBuys, "Buy increases by one");

  // test every player draws a card except player who played card
  testPassed += Assert((initState->handCount[0] + 3) >= state->handCount[0], "Player 1 did not draw an extra card");
  testPassed += Assert((initState->handCount[1] + 1) == state->handCount[1], "Player 2 drew a card");
  testPassed += Assert((initState->handCount[2] + 1) == state->handCount[2], "Player 3 drew a card");
  testPassed += Assert((initState->handCount[3] + 1) == state->handCount[3], "Player 4 drew a card");

  // test card is discarded
  int i = 0;
  bool isCardDiscarded = true;
  for (i = 0; i < state->handCount[player]; i++) {
    if (state->hand[player][i] == council_room) {
      isCardDiscarded = false;
    }
  }
  testPassed += Assert(isCardDiscarded, "Card is discarded");

  // test supply piles stay the same
  i = 0;
  bool isSameCount = true;
  for (i = 0; i <= treasure_map; i++) {
    if (state->supplyCount[i] != initState->supplyCount[i]) {
      isSameCount = false;
    }
  }
  testPassed += Assert(isSameCount, "Supply piles stay the same");

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
  int testResults = testCouncilRoom(&G, &initialG);

  if (testResults > 0)
    printf("TEST FAILED\n\n");
  else
    printf("TEST SUCCESSFULLY COMPLETED\n\n");

  return 0;
}