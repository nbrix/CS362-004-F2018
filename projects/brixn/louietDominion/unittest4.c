/*****************************************************************************
Name: Nikolas Brix
Date: 10/28/2018
Descripton: Unit test for whoseTurn function.
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

int testWhoseTurn(struct gameState *state)
{
  int testPassed = 0;
  int player = 0;

  // test correct turn is returned
  state->whoseTurn = player;
  printf("--------- START OF UNIT TEST 4 ---------\n");
  testPassed += Assert((whoseTurn(state) == player), "Returns correct turn for player 1");
  state->whoseTurn = 1;
  testPassed += Assert((whoseTurn(state) == 1), "Returns correct turn for player 2");
  state->whoseTurn = 2;
  testPassed += Assert((whoseTurn(state) == 2), "Returns correct turn for player 3");
  state->whoseTurn = 3;
  testPassed += Assert((whoseTurn(state) == 3), "Returns correct turn for player 4");
  state->whoseTurn = 4;
  testPassed += Assert((whoseTurn(state) != 4), "Nonexistent player's turn");

  return testPassed;
}

int main()
{
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
  int numPlayers = NUM_PLAYERS;

  srand(time(0));

  initializeGame(numPlayers, k, rand() % 1000, &G);
  int testResults = testWhoseTurn(&G);

  if (testResults > 0)
    printf("TEST FAILED\n\n");
  else
    printf("TEST SUCCESSFULLY COMPLETED\n\n");

  return 0;
}