/*****************************************************************************
Name: Nikolas Brix
Date: 10/28/2018
Descripton: Unit test for isGameOver function.
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

int testIsGameOver(struct gameState *state)
{
  int testPassed = 0;

  // test when there are province cards left and less than 3 supply piles that are empty
  printf("--------- START OF UNIT TEST 2 ---------\n");
  testPassed += Assert(!isGameOver(state), "Normal game conditions");

  state->supplyCount[province] = 0;
  // test if there are no more province cards
  testPassed += Assert(isGameOver(state), "No Province cards left");

  // test when three piles are empty
  state->supplyCount[province] = 10;
  state->supplyCount[4] = 0;
  state->supplyCount[5] = 0;
  state->supplyCount[6] = 0;

  // If three supply piles are empty, the game ends with 3 or less people. 
  // However, the game continues with 4 or more people.
  state->numPlayers = 3;
  testPassed += Assert(isGameOver(state), "3 supply piles are empty for game with 3 or less people");

  state->numPlayers = 4;
  testPassed += Assert(!isGameOver(state), "3 supply piles are empty for game with 4 or more people");
  
  // test if game ends with 4 supply piles empty
  state->supplyCount[7] = 0;
  testPassed += Assert(isGameOver(state), "4 supply piles are empty");

  return testPassed;
}

int main()
{
  struct gameState G;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
  int numPlayers = MIN_PLAYERS;

  srand(time(0));

  initializeGame(numPlayers, k, rand() % 1000, &G);
  int testResults = testIsGameOver(&G);

  if (testResults > 0)
    printf("TEST FAILED\n\n");
  else
    printf("TEST SUCCESSFULLY COMPLETED\n\n");

  return 0;
}