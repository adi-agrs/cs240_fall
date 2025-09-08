/*
 * Homework 1
 * Adi Agrawal
 * CS 240, Fall 2025
 * Purdue University
 */

#include "hw1.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_PLAYERS (10)
#define MAX_CARDS (52)
#define MAX_CARDS_PER_HAND (26)
#define ERROR (-2)
#define INVALID_CARD (-3)
#define OK (-1)
#define WIN (7)
#define LOSE (8)
#define TIE (9)
#define FOUR_OF_A_KIND (6)
#define FULL_HOUSE (4)
#define THREE_OF_A_KIND (3)
#define TWO_PAIR (2)
#define PAIR (1)
#define HIGH_CARD (0)

char g_shuffled_deck[MAX_CARDS] = {"23456789XKQAJ23456789XKQAJ23456789XKQAJ23456789XKQAJ"};
char g_card_hands[MAX_PLAYERS][MAX_CARDS_PER_HAND] = {};
int g_game_results[MAX_PLAYERS] = {};

/* Write your code here */

/**
 * check_deck - validate all cards in g_shuffled_deck
 * returns INVALID_CARD if any card is not recognized,
 * otherwise returns OK.
 */
int check_deck() {
  //loop through all cards in deck
  for (int i = 0; i < strlen(g_shuffled_deck); i++) {
    //changed MAX_CARDS to strlen(g_shuffled_deck) = doesnt work because not null terminated ?
    //lower ASCII than a 2 number card means invalid card
    if (g_shuffled_deck[i] < 50) {
      return INVALID_CARD;
    }
    if (g_shuffled_deck[i] > 57) {
      //if its not a number card and isnt one of the face cards return INVALID CARD;
      if (g_shuffled_deck[i] != 'J' && g_shuffled_deck[i] != 'Q' && g_shuffled_deck[i] != 'A' && g_shuffled_deck[
            i] != 'K'
          && g_shuffled_deck[i] != 'X') {
        return INVALID_CARD;
      }
    }
  }
  return OK; //return ok if no invalid cards
}

int check_player_cards(int player_index, int num_of_cards) {
  for (int i = 0; i < num_of_cards; i++) {
    if (g_card_hands[player_index][i] < 50) {
      return INVALID_CARD;
    }
    if (g_card_hands[player_index][i] > 57) {
      if (g_card_hands[player_index][i] != 'J' && g_card_hands[player_index][i] != 'Q' && g_card_hands[player_index][i]
        != 'A' && g_card_hands[player_index][i] != 'K' && g_card_hands[player_index][i] != 'X') {
        return INVALID_CARD;
      }
    }
  }
  return OK;
}

/**
 * blackjack_val - return the value of a card for blackjack
 * J, Q, K, X = 10
 * A = 11
 * 2–9 = numeric value
 */
int blackjack_val(char card) {
  if (card == 'J' || card == 'Q' || card == 'K' || card == 'X') {
    return 10;
  }
  if (card == 'A') {
    return 11;
  }
  if (card >= 50 && card <= 57) {
    return card - 48;
  }
  return INVALID_CARD;
}

/**
 * war_card_comp - assign numeric values for War comparisons
 * 2–9 -> ASCII Values starting at 50
 * X = 58, J = 59, Q = 60, K = 61, A = 62
 */
int war_card_comp(char card) {
  if (card >= 50 && card <= 57) {
    return card;
  }
  if (card == 'X') {
    return 58;
  }
  if (card == 'J') {
    return 59;
  }
  if (card == 'Q') {
    return 60;
  }
  if (card == 'K') {
    return 61;
  }
  if (card == 'A') {
    return 62;
  }
  return INVALID_CARD;
}

/**
 * check_four_of_a_kind - return true if hand has 4 of same rank
 */
bool check_four_of_a_kind(char cards_in_a_hand[5]) {
  //loop through every card in a hand
  int rank_duplicates = 0;
  for (int i = 0; i < 5; i++) {
    //first card = cards_in_a_hand[0]
    for (int j = 0; j < 5; j++) {
      // checks the (i) card against every other card
      if (cards_in_a_hand[i] == cards_in_a_hand[j]) {
        rank_duplicates++;
      }
    }
    if (rank_duplicates < 4) {
      rank_duplicates = 0;
    }
  }
  if (rank_duplicates >= 4) {
    return true;
  }
  return false;
}

/**
 * check_three_of_a_kind - return true if hand has 3 of same rank
 */
bool check_three_of_a_kind(char cards_in_a_hand[5]) {
  int rank_duplicates = 0;
  for (int i = 0; i < 5; i++) {
    //first card = cards_in_a_hand[0]
    for (int j = 0; j < 5; j++) {
      // checks the (i) card against every other card
      if (cards_in_a_hand[i] == cards_in_a_hand[j]) {
        rank_duplicates++;
      }
    }
    if (rank_duplicates < 3) {
      rank_duplicates = 0;
    }
    if (rank_duplicates == 3) {
    return true;
    }
  }

  return false;
}

//22344 47435 38638

/**
 * check_two_pair - return true if hand has exactly 2 pairs
 */
bool check_two_pair(char cards_in_a_hand[5]) {
  int rank_duplicates = 0;
  int number_of_pairs = 0;
  char pair_one = '';

  for (int i = 0; i < 5; i++) {
    if (cards_in_a_hand[i] != pair_one) {
      //first card = cards_in_a_hand[0]
      for (int j = i + 1; j < 5; j++) {
        // checks the (i) card against every other card
        if (cards_in_a_hand[i] == cards_in_a_hand[j]) {
          rank_duplicates++;
        }
      }
      if (rank_duplicates != 2) {
        rank_duplicates = 0;
      }
      if (rank_duplicates == 2) {
        number_of_pairs++;
        pair_one = cards_in_a_hand[i];
      }
    }
  }
  if (number_of_pairs == 2) {
    return true;
  }
  return false;
}

bool check_pair(char cards_in_a_hand[5]) {
  int rank_duplicates = 0;
  int number_of_pairs = 0;
  char pair_one = '';

  for (int i = 0; i < 5; i++) {
    if (cards_in_a_hand[i] != pair_one) {
      //first card = cards_in_a_hand[0]
      for (int j = i + 1; j < 5; j++) {
        // checks the (i) card against every other card
        if (cards_in_a_hand[i] == cards_in_a_hand[j]) {
          rank_duplicates++;
        }
      }
      if (rank_duplicates != 2) {
        rank_duplicates = 0;
      }
      if (rank_duplicates == 2) {
        number_of_pairs++;
        pair_one = cards_in_a_hand[i];
      }
    }
  }
  if (number_of_pairs == 1) {
    return true;
  }
  return false;
}


/**
 * check_full_house - return true if hand has a 3 of a kind and a pair
 */
bool check_full_house(char cards_in_a_hand[5]) {
  if (check_three_of_a_kind(cards_in_a_hand) == true &&
      check_pair(cards_in_a_hand) == true) {
    return true;
  }
  return false;
}

/**
 * check_hand - evaluate a hand and update g_game_results
 */

//HERE!!!! == true
int check_hand(int player_index, char cards_in_a_hand[5]) {
  if (check_four_of_a_kind(cards_in_a_hand) == true) {
    g_game_results[player_index] = FOUR_OF_A_KIND;
  } else if (check_full_house(cards_in_a_hand) == true) {
    g_game_results[player_index] = FULL_HOUSE;
  } else if (check_three_of_a_kind(cards_in_a_hand) == true) {
    g_game_results[player_index] = THREE_OF_A_KIND;
  } else if (check_two_pair(cards_in_a_hand) == true) {
    g_game_results[player_index] = TWO_PAIR;
  } else if (check_pair(cards_in_a_hand) == true) {
    g_game_results[player_index] = PAIR;
  } else {
    g_game_results[player_index] = HIGH_CARD;
  }
  return g_game_results[player_index];
}

///HOMEWORK FUNCTIONS

/**
 * deal_cards - deal cards_per_hand cards to each of num_players
 * copies from g_shuffled_deck into g_card_hands
 */
int deal_cards(int num_players, int cards_per_hand) {
  if (check_deck() == INVALID_CARD) {
    return INVALID_CARD;
  }
  //if the input parameters exceed the maximum amount - fix comment
  if (cards_per_hand > MAX_CARDS_PER_HAND || num_players > MAX_PLAYERS || num_players < 1 ||cards_per_hand < 1 ) {
    return ERROR;
  }
  if ((num_players*cards_per_hand) > MAX_CARDS) {
    return ERROR;
  }
  int card_index = 0;
  for (int i = 0; i < cards_per_hand; i++) {
    for (int j = 0; j < num_players; j++) {
      g_card_hands[j][i] = g_shuffled_deck[card_index++]; //assigns shuffled deck cards at index j to the hands
    }
  }
  return OK;
}

/**
 * war_sim - simulate the game War with 2 players
 * returns WIN if player 0 wins, LOSE if player 1 wins, TIE if equal
 */
int war_sim(int cards_per_hand) {
  memset(g_game_results, 0, sizeof(g_game_results));  // reset results
  if (cards_per_hand > MAX_CARDS_PER_HAND) {
    return ERROR;
  }
  if (cards_per_hand < 1) {
    return ERROR;
  }
  for (int i = 0; i < 2; i++) { //two players max
    if (check_player_cards(i,cards_per_hand) == INVALID_CARD) {
      return INVALID_CARD;
    }
  }
  for (int i = 0; i < cards_per_hand; i++) {
    //number of rounds. each loop is one round
    //tie case
    if (war_card_comp(g_card_hands[0][i]) == war_card_comp(g_card_hands[1][i])) {
      //increment both players victory counts by 1
      g_game_results[0]++;
      g_game_results[1]++;
    }
    //player 1 wins
    else if (war_card_comp(g_card_hands[0][i]) > war_card_comp(g_card_hands[1][i])) {
      g_game_results[0]++;
    }
    //player 2 wins
    else {
      g_game_results[1]++;
    }
  } //round over
  if (g_game_results[0] > g_game_results[1]) {
    return WIN;
  } else if (g_game_results[0] < g_game_results[1]) {
    return LOSE;
  }
  return TIE;
}

/**
 * blackjack_sim - simulate Blackjack hands against dealer
 * each player gets 3 cards
 * returns total number of wins + ties
 */
int blackjack_sim(int num_players, int dealer_total) {
  memset(g_game_results, 0, sizeof(g_game_results));
  //return number of wins and ties
  int total_wins_and_ties = 0;
  ///HERE!!!!!!
  if (num_players > MAX_PLAYERS  || num_players < 1 || dealer_total < 2) {
    return ERROR;
  }
  //REMOVED DEAL CARDS!!!
  for (int i = 0; i < num_players; i++) {
    if (check_player_cards(i,3) == INVALID_CARD) {
      return INVALID_CARD;
    }
  }

  for (int i = 0; i < num_players; i++) {
    //each player
    int total_value_of_cards = 0;
    for (int j = 0; j < 3; j++) {
      //each card (3 cards)
      if (blackjack_val(g_card_hands[i][j]) == INVALID_CARD) {
        return INVALID_CARD;
      }
      total_value_of_cards += blackjack_val(g_card_hands[i][j]);
    } //here

    if (dealer_total > 21 && total_value_of_cards <= 21) {
      g_game_results[i] = WIN;
      total_wins_and_ties++;
    } else if (total_value_of_cards <= 21 && total_value_of_cards > dealer_total) {
      //WIN
      g_game_results[i] = WIN;
      total_wins_and_ties++;
    } else if (total_value_of_cards <= 21 && total_value_of_cards == dealer_total) {
      //TIE
      g_game_results[i] = TIE;
      total_wins_and_ties++;
    } else {
      g_game_results[i] = LOSE;
    }
  } //games over for each player
  return total_wins_and_ties;
}

/**
 * poker_sim - simulate Poker hands
 * deals 5 cards to each player and returns the number of hands better than HIGH_CARD
 */
int poker_sim(int num_players) {
  memset(g_game_results, 0, sizeof(g_game_results));
  int number_of_hands_better_than_high_card = 0;

  //HERE!!!!!!
  if (num_players > MAX_PLAYERS || num_players < 1) {
    return ERROR;
  }
  // if (deal_cards(num_players, 5) == INVALID_CARD) {
  //   return INVALID_CARD;
  // }
  for (int i = 0; i < num_players; i++) {
    if (check_player_cards(i,5) == INVALID_CARD) {
      return INVALID_CARD;
    }
  }
  for (int i = 0; i < num_players; i++) {
    if ((check_hand(i, g_card_hands[i])) != HIGH_CARD) {
      number_of_hands_better_than_high_card++;
    };
  }
  return number_of_hands_better_than_high_card;
}


