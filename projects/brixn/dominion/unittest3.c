/*****************************************************************************
Name: Nikolas Brix
Date: 10/28/2018
Descripton: Unit test for gainCard function.
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

int testGainCard(struct gameState *state, struct gameState *initState)
{
  int testPassed = 0;
  int locationToAddCard = 2;
  int player = 0;
  int initHandCount = state->handCount[player];
  int initDeckCount = state->deckCount[player];
  int initDiscardCount = state->discardCount[player];
  int initSupplyCount = state->supplyCount[adventurer];

  // test card is put in the player's hand and hand count increases
  gainCard(adventurer, state, locationToAddCard, player);
  printf("--------- START OF UNIT TEST 3 ---------\n");
  testPassed += Assert((state->hand[player][state->handCount[player] - 1] == adventurer), "Card is added to player's hand");
  testPassed += Assert(state->handCount[player] == (initHandCount + 1), "Player's hand increases by 1");

  // test supply is uneffected except by the one card that is takem
  int i = 0;
  bool isSameCount = true;
  for (i = 0; i <= treasure_map; i++) {
    if (state->supplyCount[i] != initState->supplyCount[i]) {
      if (i != adventurer) {
        isSameCount = false;
      }
    }
  }
  testPassed += Assert((state->supplyCount[adventurer] == (initSupplyCount - 1)) && isSameCount, "Only card supply that is taken is decremented");

  // test card is put into player's deck
  locationToAddCard = 1;
  gainCard(adventurer, state, locationToAddCard, player);
  testPassed += Assert((state->deck[player][state->deckCount[player] - 1] == adventurer), "Card is added to player's deck");
  testPassed += Assert(state->deckCount[player] == (initDeckCount + 1), "Player's deck increases by 1");

  // test if card is added to discard pile
  locationToAddCard = 0;
  gainCard(adventurer, state, locationToAddCard, player);
  testPassed += Assert((state->discard[player][state->discardCount[player] - 1] == adventurer), "Card is added to player's discard pile");
  testPassed += Assert(state->discardCount[player] == (initDiscardCount + 1), "Player's discard pile increases by 1");

  // test if supply is too low for card
  state->supplyCount[adventurer] = 0;
  testPassed += Assert(gainCard(adventurer, state, locationToAddCard, player) < 0, "Supply too low");

  return testPassed;
}

int main()
{
  struct gameState G, initG;
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
  int numPlayers = MIN_PLAYERS;

  srand(time(0));

  initializeGame(numPlayers, k, rand() % 1000, &G);
  memcpy(&initG, &G, sizeof(struct gameState));
  int testResults = testGainCard(&G, &initG);

  if (testResults > 0)
    printf("TEST FAILED\n\n");
  else
    printf("TEST SUCCESSFULLY COMPLETED\n\n");

  return 0;
}