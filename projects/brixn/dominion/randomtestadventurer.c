/*****************************************************************************
Name: Nikolas Brix
Date: 11/12/2018
Descripton: Random test for Adventurer card.
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

int testAdventurer(struct gameState *state, struct gameState *initState, int numPlayers)
{
  int testPassed = 0;
  int placeHolder = 0;
  int *bonus = &placeHolder;
  int cards[16] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, copper, silver, gold, estate, duchy, province,};

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

  // count number of treasures
  int numTreasuresInHand = 0;
  for (i = 0; i < handcount; i++) {
  	if (state->hand[player][i] == copper || state->hand[player][i] == silver || state->hand[player][i] == gold) {
  		numTreasuresInHand++;
  	}
  }

  // count number of treasures in deck
  int numTreasuresInDeck = 0;
  for (i = 0; i < deckcount; i++) {
  	if (state->deck[player][i] == copper || state->deck[player][i] == silver || state->deck[player][i] == gold) {
  		numTreasuresInDeck++;
  	}
  }

  int numAdventureInHand = 0;
  for (i = 0; i < handcount; i++) {
  	if (state->hand[player][i] == adventurer) {
  		numAdventureInHand++;
  	}
  }

  cardEffect(adventurer, placeHolder, placeHolder, placeHolder, state, placeHolder, bonus);
  
  int currentHandCount = state->handCount[player];

  // count number of treasures in hand
  int numTreasures = 0;
  for (i = 0; i < currentHandCount; i++) {
    int card = state->hand[player][i];
    if (card == copper || card == silver || card == gold) {
      numTreasures++;
    }
  }

  // test if no treasures are found - should be impossible scenario in a real game
  if (numTreasuresInDeck == 0)
  	testPassed += Assert((numTreasuresInHand) == numTreasures, "No treasure was found");

  // test if only 1 treasure is found, hand stays the same
  if (numTreasuresInDeck == 1)
  	testPassed += Assert((numTreasuresInHand + 1) == numTreasures, "One treasure is found");

  // test if 2 treasures are in the deck
  if (numTreasuresInDeck >= 2)
  	testPassed += Assert((numTreasuresInHand + 2) == numTreasures, "Two treasures are found");

  // test supply piles stay the same
  bool isSameCount = true;
  for (i = 0; i <= treasure_map; i++) {
    if (state->supplyCount[i] != initState->supplyCount[i]) {
      isSameCount = false;
    }
  }
  testPassed += Assert(isSameCount, "Supply piles stay the same");

  // test Adventurer card is discarded
  int finalAdventureCount = 0;
  for (i = 0; i < state->handCount[player]; i++) {
    if (state->hand[player][i] == adventurer) {
      finalAdventureCount++;
    }
  }
  testPassed += Assert(finalAdventureCount == (numAdventureInHand - 1), "Card is discarded");
  
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
  	int testResults = testAdventurer(&G, &initialG, numPlayers);

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