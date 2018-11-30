/*****************************************************************************
Name: Nikolas Brix
Date: 11/12/2018
Descripton: Random test for Council Room card.
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

int testCouncilRoom(struct gameState *state, struct gameState *initState, int numPlayers)
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

  // count number of Council Rooms in hand
  int handpos = 0;
  int numCouncilRoomInHand = 0;
  for (i = 0; i < handcount; i++) {
  	if (state->hand[player][i] == council_room) {
  		numCouncilRoomInHand++;
      handpos = i;
  	}
  }

  // if there are no council rooms in hand, put one in the first position
  if (numCouncilRoomInHand == 0) {
    state->hand[player][0] = council_room;
    numCouncilRoomInHand = 1;
  }

  // get initial hand counts of all players
  int initHandCount[4];
  for (i = 0; i < numPlayers; i++) {
    initHandCount[i] = state->handCount[i];
  }

  cardEffect(council_room, placeHolder, placeHolder, placeHolder, state, handpos, bonus);
  
  // get current hand count of all players
  int currentHandCount[4];
  for (i = 0; i < numPlayers; i++) {
    currentHandCount[i] = state->handCount[i];
  }

  // test if hand increases by three
  testPassed += Assert((currentHandCount[player] - 3) == initHandCount[player], "Draw four cards");

  // test if other players hands only increase by one
  for (i = 0; i < numPlayers; i++) {
    if (i != player)
      testPassed += Assert((currentHandCount[i] - 1) == initHandCount[i], "Draw one card");
  }

  // test state increases buy by one
  testPassed += Assert((initState->numBuys + 1) == state->numBuys, "Buy increases by one");
  
  // test supply piles stay the same
  bool isSameCount = true;
  for (i = 0; i <= treasure_map; i++) {
    if (state->supplyCount[i] != initState->supplyCount[i]) {
      isSameCount = false;
    }
  }
  testPassed += Assert(isSameCount, "Supply piles stay the same");

  // test coucil room card is discarded
  int finalCouncilRoomCount = 0;
  for (i = 0; i < state->handCount[player]; i++) {
    if (state->hand[player][i] == council_room) {
      finalCouncilRoomCount++;
    }
  }
  testPassed += Assert(finalCouncilRoomCount == (numCouncilRoomInHand - 1), "Card is discarded");
  
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
  	int testResults = testCouncilRoom(&G, &initialG, numPlayers);

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