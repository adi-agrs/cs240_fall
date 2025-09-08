/*
 * Homework 1
 * Adi Agrawal
 * CS 240, Fall 2025
 * Purdue University
 */

#include "hw1.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

char g_shuffled_deck[MAX_CARDS] = {};
char g_card_hands[MAX_PLAYERS][MAX_CARDS_PER_HAND] = {};
int g_game_results[MAX_PLAYERS] = {};

/**
 * check_deck - validate all cards in g_shuffled_deck
 * returns INVALID_CARD if any card is not recognized,
 * otherwise returns OK.
 */

int check_deck() {
  /* loop through all cards in deck */
  for (int i = 0; i < strlen(g_shuffled_deck); i++) {
    /* lower ASCII than a 2 number card means invalid card */
    if (g_shuffled_deck[i] < 50) {
      return INVALID_CARD;
    }
    if (g_shuffled_deck[i] > 57) {
      if ((g_shuffled_deck[i] != 'J')
        && (g_shuffled_deck[i] != 'Q')
        && (g_shuffled_deck[i] != 'A')
        && (g_shuffled_deck[i] != 'K')
        && (g_shuffled_deck[i] != 'X')) {
        return INVALID_CARD;
      }
    }
  }
  return OK;
} /* check_deck() */

/**
 * check_player_cards - validate all cards in g_card_hands for a player
 * returns INVALID_CARD if any card is not recognized,
 * otherwise returns OK.
 */

int check_player_cards(int player_index, int num_of_cards) {
  for (int i = 0; i < num_of_cards; i++) {
    /* less than ASCII of '2' */
    if (g_card_hands[player_index][i] < 50) {
      return INVALID_CARD;
    }
    /* more than ASCII of '9' */
    if (g_card_hands[player_index][i] > 57) {
      if ((g_card_hands[player_index][i] != 'J')
        && (g_card_hands[player_index][i] != 'Q')
        && (g_card_hands[player_index][i] != 'A')
        && (g_card_hands[player_index][i] != 'K')
        && (g_card_hands[player_index][i] != 'X')) {
        return INVALID_CARD;
      }
    }
  }
  return OK;
} /* check_player_cards() */

/**
 * blackjack_val - return the value of a card for blackjack
 * J, Q, K, X = 10
 * A = 11
 * 2–9 = numeric value
 */

int blackjack_val(char card) {
  if ((card == 'J')
    || (card == 'Q')
    || (card == 'K')
    || (card == 'X')) {
    return 10;
  }
  if (card == 'A') {
    return 11;
  }
  /* if between '2' and '9' return numeric value
   * by subtracting 48 from the ASCII value */
  if ((card >= 50) && (card <= 57)) {
    return card - 48;
  }
  return INVALID_CARD;
} /* blackjack_val() */

/**
 * war_card_comp - assign numeric values for War comparisons
 * 2–9 -> ASCII Values starting at 50
 * X = 58, J = 59, Q = 60, K = 61, A = 62
 */

int war_card_comp(char card) {
  if ((card >= 50) && (card <= 57)) {
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
} /* war_card_comp() */

/**
 * check_four_of_a_kind - return true if hand has 4 of same rank
 */

bool check_four_of_a_kind(char cards_in_a_hand[5]) {
  /* loop through every card in a hand */
  int rank_duplicates = 0;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      /* checks the (i) card against every other card */
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
} /* check_four_of_a_kind() */

/**
 * check_three_of_a_kind - return true if hand has 3 of same rank
 */

bool check_three_of_a_kind(char cards_in_a_hand[5]) {
  int rank_duplicates = 0;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      /* checks the (i) card against every other card */
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
} /* check_three_of_a_kind() */

/**
 * check_two_pair - return true if hand has exactly 2 pairs
 */

bool check_two_pair(char cards_in_a_hand[5]) {
  int rank_duplicates = 0;
  int first_pair_ascii = 0;
  /* picking a card */
  for (int i = 0; i < 5; i++) {
    /* comparing to every card afterward */
    for (int j = i + 1; j < 5; j++) {
      if (cards_in_a_hand[i] == cards_in_a_hand[j]) {
        rank_duplicates++;
        /* stores the cards which are in a pair */
        first_pair_ascii = cards_in_a_hand[i];
        break;
      }
    }
    if (first_pair_ascii != 0) {
      break;
    }
  }
  /*loop to find the second pair */
  for (int i = 0; i < 5; i++) {
    if (cards_in_a_hand[i] == first_pair_ascii) {
      /* skips card */
      continue;
    }
    for (int j = i + 1; j < 5; j++) {
      if (cards_in_a_hand[i] == cards_in_a_hand[j]) {
        rank_duplicates++;
        break;
      }
    }
  }
  if (rank_duplicates == 2) {
    return true;
  }
  return false;
} /* check_two_pair() */

/**
 * check_pair - return true if hand has exactly 1 pair
 */

bool check_pair(char cards_in_a_hand[5]) {
  int rank_duplicates = 0;
  /* picking a card */
  for (int i = 0; i < 5; i++) {
    rank_duplicates = 0;
    /* comparing to every card afterward */
    for (int j = i + 1; j < 5; j++) {
      if (cards_in_a_hand[i] == cards_in_a_hand[j]) {
        rank_duplicates++;
      }
    }
    if (rank_duplicates == 1) {
      /* if not a pair continue to next card */
      break;
    }
  }
  if (rank_duplicates == 1) {
    return true;
  }
  return false;
} /* check_pair() */

/**
 * check_full_house - return true if hand has a 3 of a kind and a pair
 */

bool check_full_house(char cards_in_a_hand[5]) {
  if ((check_three_of_a_kind(cards_in_a_hand) == true) &&
      (check_pair(cards_in_a_hand) == true)) {
    return true;
  }
  return false;
} /* check_full_house() */

/**
 * check_hand - evaluate a hand and update g_game_results
 */

int check_hand(int player_index, char cards_in_a_hand[5]) {
  if (check_four_of_a_kind(cards_in_a_hand) == true) {
    g_game_results[player_index] = FOUR_OF_A_KIND;
  }
  else if (check_full_house(cards_in_a_hand) == true) {
    g_game_results[player_index] = FULL_HOUSE;
  }
  else if (check_three_of_a_kind(cards_in_a_hand) == true) {
    g_game_results[player_index] = THREE_OF_A_KIND;
  }
  else if (check_two_pair(cards_in_a_hand) == true) {
    g_game_results[player_index] = TWO_PAIR;
  }
  else if (check_pair(cards_in_a_hand) == true) {
    g_game_results[player_index] = PAIR;
  }
  else {
    g_game_results[player_index] = HIGH_CARD;
  }
  return g_game_results[player_index];
} /* check_hand() */

/**
 * deal_cards - deal cards_per_hand cards to each of num_players
 * copies from g_shuffled_deck into g_card_hands
 */

int deal_cards(int num_players, int cards_per_hand) {
  if (check_deck() == INVALID_CARD) {
    return INVALID_CARD;
  }
  if ((cards_per_hand > MAX_CARDS_PER_HAND) || (num_players > MAX_PLAYERS)
    || (num_players < 1) || (cards_per_hand < 1)) {
    return ERROR;
  }
  if ((num_players * cards_per_hand) > MAX_CARDS) {
    return ERROR;
  }
  int card_index = 0;
  for (int i = 0; i < cards_per_hand; i++) {
    for (int j = 0; j < num_players; j++) {
      g_card_hands[j][i] = g_shuffled_deck[card_index++];
    }
  }
  return OK;
} /* deal_cards() */

/**
 * war_sim - simulate the game War with 2 players
 * returns WIN if player 0 wins, LOSE if player 1 wins, TIE if equal
 */

int war_sim(int cards_per_hand) {
  /* reset results */
  memset(g_game_results, 0, sizeof(g_game_results));
  if (cards_per_hand > MAX_CARDS_PER_HAND) {
    return ERROR;
  }
  if (cards_per_hand < 1) {
    return ERROR;
  }
  /* always 2 players */
  for (int i = 0; i < 2; i++) {
    if (check_player_cards(i, cards_per_hand) == INVALID_CARD) {
      return INVALID_CARD;
    }
  }
  for (int i = 0; i < cards_per_hand; i++) {
    /* ties */
    if (war_card_comp(g_card_hands[0][i])
      == war_card_comp(g_card_hands[1][i])) {
      g_game_results[0]++;
      g_game_results[1]++;
    }
    else if (war_card_comp(g_card_hands[0][i])
      > war_card_comp(g_card_hands[1][i])) {
      /* player 1 wins */
      g_game_results[0]++;
    }
    else {
      /* player 2 wins player 1 loses */
      g_game_results[1]++;
    }
  }
  if (g_game_results[0] > g_game_results[1]) {
    return WIN;
  }
  else if (g_game_results[0] < g_game_results[1]) {
    return LOSE;
  }
  return TIE;
} /* war_sim() */

/**
 * blackjack_sim - simulate Blackjack hands against dealer
 * each player gets 3 cards
 * returns total number of wins + ties
 */

int blackjack_sim(int num_players, int dealer_total) {
  /* reset results */
  memset(g_game_results, 0, sizeof(g_game_results));
  int total_wins_and_ties = 0;
  if ((num_players > MAX_PLAYERS)
    || (num_players < 1)
    || (dealer_total < 2)) {
    return ERROR;
  }
  for (int i = 0; i < num_players; i++) {
    if (check_player_cards(i, 3) == INVALID_CARD) {
      return INVALID_CARD;
    }
  }
  for (int i = 0; i < num_players; i++) {
    int total_value_of_cards = 0;
    for (int j = 0; j < 3; j++) {
      if (blackjack_val(g_card_hands[i][j]) == INVALID_CARD) {
        return INVALID_CARD;
      }
      total_value_of_cards += blackjack_val(g_card_hands[i][j]);
    }
    /** WIN
    * if dealer busts and player doesn't.
    * Player wins even if hand is lower
    * than dealers if busted */
    if ((dealer_total > 21) && (total_value_of_cards <= 21) ||
      ((total_value_of_cards <= 21)
        && (total_value_of_cards > dealer_total))) {
      g_game_results[i] = WIN;
      total_wins_and_ties++;
    }
    else if ((total_value_of_cards <= 21)
      && (total_value_of_cards == dealer_total)) {
      /* TIE */
      g_game_results[i] = TIE;
      total_wins_and_ties++;
    }
    else {
      /* LOSE */
      g_game_results[i] = LOSE;
    }
  }
  return total_wins_and_ties;
} /* blackjack_sim() */

/**
 * poker_sim - simulate Poker hands
 * deals 5 cards to each player and
 * returns the number of hands better than HIGH_CARD
 */

int poker_sim(int num_players) {
  /* reset results */
  memset(g_game_results, 0, sizeof(g_game_results));
  int number_of_hands_better_than_high_card = 0;
  if ((num_players > MAX_PLAYERS)
    || (num_players < 1)) {
    return ERROR;
  }
  for (int i = 0; i < num_players; i++) {
    if (check_player_cards(i, 5) == INVALID_CARD) {
      return INVALID_CARD;
    }
  }
  for (int i = 0; i < num_players; i++) {
    if ((check_hand(i, g_card_hands[i])) != HIGH_CARD) {
      number_of_hands_better_than_high_card++;
    };
  }
  return number_of_hands_better_than_high_card;
} /* poker_sim() */
