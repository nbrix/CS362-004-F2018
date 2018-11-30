/*****************************************************************************
Name: Nikolas Brix
Date: 10/28/2018
Descripton: Unit test for Adventurer card.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./dominion.h"
#include "./dominion_helpers.h"
#include "./rngs.h"

#define NUM_PLAYERS 2

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

int testAdventurer(struct gameState *state, struct gameState *initState)
{
  int testPassed = 0;
  int player = 0;
  int placeHolder = 0;
  int *bonus = &placeHolder;
  int numInitialTreasures = 5;

  // initialize deck to have no treasures
  int i = 0;
  int initHandCount = state->handCount[player];
  for (i = 0; i < initHandCount; i++) {
    state->hand[player][i] = copper;
  }
  for (i = 0; i < state->deckCount[player]; i++) {
    state->deck[player][i] = province;
  }

  cardEffect(adventurer, placeHolder, placeHolder, placeHolder, state, placeHolder, bonus);
  
  // test if no treasures are found, hand stays the same
  int currentHandCount = state->handCount[player];
  printf("--------- START OF CARD TEST 1 ---------\n");
  testPassed += Assert((initHandCount) == currentHandCount, "No treasures are found");

  // initialize deck to have 1 treasures
  state->handCount[player] = 5;
  state->deckCount[player] = 5;
  state->discardCount[player] = 0;
  initHandCount = state->handCount[player];
  for (i = 0; i < initHandCount; i++) {
    state->hand[player][i] = copper;
  }
  for (i = 1; i < state->deckCount[player]; i++) {
    state->deck[player][i] = province;
  }
  state->deck[player][0] = copper;

  cardEffect(adventurer, placeHolder, placeHolder, placeHolder, state, placeHolder, bonus);
  
  // test if only 1 treasure is found, hand stays the same
  currentHandCount = state->handCount[player];
  testPassed += Assert((initHandCount + 1) == currentHandCount, "One treasure is found");

  // initialize deck to have 2 treasures on top
  state->handCount[player] = 5;
  state->deckCount[player] = 5;
  state->discardCount[player] = 0;
  initHandCount = state->handCount[player];
  for (i = 0; i < initHandCount; i++) {
    state->hand[player][i] = copper;
  }
  for (i = 0; i < state->deckCount[player] - 2; i++) {
    state->deck[player][i] = province;
  }
  state->deck[player][state->deckCount[player] - 2] = copper;
  state->deck[player][state->deckCount[player] - 1] = copper;

  cardEffect(adventurer, placeHolder, placeHolder, placeHolder, state, placeHolder, bonus);

  // count number of treasures in hand
  int numTreasures = 0;
  currentHandCount = state->handCount[player];
  for (i = 0; i < currentHandCount; i++) {
    int card = state->hand[player][i];
    if (card == copper || card == silver || card == gold) {
      numTreasures++;
    }
  }

  // test number of treasures increased by 2
  testPassed += Assert((numInitialTreasures + 2) == numTreasures, "Player has two more treasures in hand with treasures on top of deck");

  // test hand count increases by 2
  testPassed += Assert((initHandCount + 2) == currentHandCount, "Player's hand has increased by 2 with treasures on top of deck");

  // initialize deck to have 2 treasures on bottom
  state->handCount[player] = 5;
  state->deckCount[player] = 5;
  state->discardCount[player] = 0;
  initHandCount = state->handCount[player];
  for (i = 0; i < initHandCount; i++) {
    state->hand[player][i] = copper;
  }
  for (i = 2; i < state->deckCount[player]; i++) {
    state->deck[player][i] = province;
  }
  state->deck[player][0] = copper;
  state->deck[player][1] = copper;

  cardEffect(adventurer, placeHolder, placeHolder, placeHolder, state, placeHolder, bonus);

  // test discard deck
  testPassed += Assert((state->discardCount[player]) == 3, "Discard deck"); 

  // count number of treasures in hand
  numTreasures = 0;
  
  currentHandCount = state->handCount[player];
  for (i = 0; i < currentHandCount; i++) {
    int card = state->hand[player][i];
    if (card == copper || card == silver || card == gold) {
      numTreasures++;
    }
  }

  // test number of treasures increased by 2
  testPassed += Assert((numInitialTreasures + 2) == numTreasures, "Player has two more treasures in hand with treasures on bottom of deck");

  // test hand count increases by 2
  testPassed += Assert((initHandCount + 2) == currentHandCount, "Player's hand has increased by 2 with treasures on bottom of deck");

  // test supply piles stay the same
  bool isSameCount = true;
  for (i = 0; i <= treasure_map; i++) {
    if (state->supplyCount[i] != initState->supplyCount[i]) {
      isSameCount = false;
    }
  }
  testPassed += Assert(isSameCount, "Supply piles stay the same");

  // test Adventurer card is discarded
  bool isCardDiscarded = true;
  for (i = 0; i < state->handCount[player]; i++) {
    if (state->hand[player][i] == adventurer) {
      isCardDiscarded = false;
    }
  }
  testPassed += Assert(isCardDiscarded, "Card is discarded");

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
  int testResults = testAdventurer(&G, &initialG);

  if (testResults > 0)
    printf("TEST FAILED\n\n");
  else
    printf("TEST SUCCESSFULLY COMPLETED\n\n");

  return 0;
}