/*
 * Homework 2
 * Adi Agrawal
 * CS 240, Fall 2025
 * Purdue University
 */

#include "hw2.h"

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE (1000)
#define MAX_FIELDS (11)

#define OK (0)


/*
 * data_checker - Validates the fields of a battle record.
 * Returns false if any field is invalid.
 */

bool data_checker(char *planet, char *jedi, char *clone_unit,
  int year, double duration, double cost, char outcome,
  int r_forces, int r_losses, int s_forces, int s_losses) {
  if ((planet == NULL) || (strlen(planet) == 0) ||
      (strlen(planet) > MAX_NAME_LEN) ||
      (jedi == NULL) || (strlen(jedi) == 0) ||
      (strlen(jedi) > MAX_NAME_LEN) ||
     (clone_unit == NULL) || (strlen(clone_unit) == 0) ||
      (strlen(clone_unit) > MAX_NAME_LEN)) {

    /* planet, jedi, clone_unit must not be
     * empty or longer than MAX_NAME_LEN */

    return false;
  }
  if ((year < 19) || (year > 22) || (duration <= 0.0) ||
    (duration >= 8760.0) || (cost <= 0.0)) {

    /* year, duration, and cost must be in their proper value ranges */

    return false;
  }
  if ((outcome != 'W') && (outcome != 'L') && (outcome != 'T')) {
    return false;
  }
  if ((r_forces <= 0) ||
      (s_forces <= 0) ||
      (r_losses < 0) ||
      (r_losses > r_forces) ||
    (s_losses < 0) || (s_losses > s_forces)) {

    /* losses can't be greater than the respective forces
     * and the forces can't be 0 */

    return false;
  }
  return true;
} /* data_checker() */

/*
 * empty_file_check - Checks if a file is empty
 * or contains only whitespace.
 * Returns NO_DATA if empty, OK otherwise.
 */

int empty_file_check(FILE *input_fp) {
  int first_char = '0';
  while ((first_char = fgetc(input_fp)) != EOF) {
    if (!isspace(first_char)) {
      break;
    }
  }
  if (first_char == EOF) {
    fclose(input_fp);
    return NO_DATA;
  }
  rewind(input_fp);

  /* Return OK for success. */

  return OK;
} /* empty_file_check() */

/*
 * casualty_ratio - Calculates the ratio of Republic
 * to Separatist casualties for a given year.
 */

double casualty_ratio(char *input_file, int year) {
  if ((year < 19) || (year > 22)) {
    return BAD_INPUT;
  }
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }
  if (empty_file_check(input_fp) == NO_DATA) {
    return NO_DATA;
  }
  char planet[] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[] = "";
  char clone_unit[MAX_SIZE] = "";
  int r_forces = 0;
  int r_losses = 0;
  int s_forces = 0;
  int s_losses = 0;
  double republic_cr = 0;
  double separatist_cr = 0;
  int number_of_battles_in_year = 0;
  double r_sum_of_forces_deployed_in_year = 0;
  double r_sum_of_forces_lost_in_year = 0;
  double s_sum_of_forces_deployed_in_year = 0;
  double s_sum_of_forces_lost_in_year = 0;
  int fields = 0;

  /* Read each battle record */

  while (1) {
    fields = fscanf
        (input_fp,
         "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                planet, &yy, &duration, &cost, &outcome,
                jedi, clone_unit,
                &r_forces, &r_losses, &s_forces, &s_losses);
    if (strlen(planet) == MAX_NAME_LEN) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != MAX_FIELDS) {
      fclose(input_fp);
      return BAD_DATA;
    }

    /* checks for bad data here */

    if (data_checker(planet, jedi, clone_unit, yy,
                     duration, cost, outcome,
                     r_forces, r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (yy == year) {
      number_of_battles_in_year++;
      r_sum_of_forces_deployed_in_year += r_forces;
      r_sum_of_forces_lost_in_year += r_losses;
      s_sum_of_forces_deployed_in_year += s_forces;
      s_sum_of_forces_lost_in_year += s_losses;
    }
  }
  fclose(input_fp);
  if (number_of_battles_in_year <= 0) {
    return NO_DATA;
  }
  republic_cr = (double)(1 + r_sum_of_forces_lost_in_year) /
                (number_of_battles_in_year +
                 r_sum_of_forces_deployed_in_year);
  separatist_cr = (double)(1 + s_sum_of_forces_lost_in_year) /
                  (number_of_battles_in_year +
                   s_sum_of_forces_deployed_in_year);
  if (separatist_cr == 0) {
    return BAD_INPUT;
  }
  return republic_cr / separatist_cr;
} /* casualty_ratio() */

/*
 * clone_unit_casualties_stdev - Calculates standard deviation of
 * casualties for a specific clone unit.
 */

double clone_unit_casualties_stdev(char *input_file, char *clone_unit){
  if ((clone_unit == NULL) || (strlen(clone_unit) == 0)) {
    return BAD_INPUT;
  }
  double sum_of_losses = 0.0;
  double avg_losses = 0.0;
  double number_of_battles_by_unit = 0.0;
  char planet[MAX_SIZE] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[MAX_SIZE] = "";
  char clone_unit_name[MAX_SIZE] = "";
  int r_forces = 0;
  int r_losses = 0;
  int s_forces = 0;
  int s_losses = 0;
  int fields = 0;

  /*  to hold r losses */

  double *casualties = NULL;
  int capacity = 10;
  int size = 0;
  casualties = (double *)malloc(capacity * sizeof(double));
  if (casualties == NULL) {
    return FILE_READ_ERR;
  }
  FILE *input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }
  if (empty_file_check(input_fp) == NO_DATA) {
    return NO_DATA;
  }

  /* Read each battle record */

  while (1) {
    fields = fscanf
        (input_fp,
         "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                planet, &yy, &duration, &cost, &outcome,
                jedi, clone_unit_name,
                &r_forces, &r_losses, &s_forces, &s_losses);
    if ((strlen(planet) == MAX_NAME_LEN) ||
        (strlen(clone_unit_name) == MAX_NAME_LEN)) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != MAX_FIELDS) {
      fclose(input_fp);
      return BAD_DATA;
    }

    /* checks for bad data here */

    if (data_checker(planet, jedi, clone_unit_name, yy,
                     duration, cost, outcome, r_forces,
                     r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (strcmp(clone_unit,clone_unit_name) == 0) {
      if (size >= capacity) {
        capacity *= 2;
        double *temp =
            (double *)realloc(casualties, capacity * sizeof(double));
        if (temp == NULL) {
          free(casualties);
          fclose(input_fp);
          return FILE_READ_ERR;
        }
        casualties = temp;
      }
      casualties[size] = r_losses;
      sum_of_losses += r_losses;
      size++;
      number_of_battles_by_unit++;
    }
  }

  /* If no battles found for this clone unit return NO_DATA */

  if (number_of_battles_by_unit == 0) {
    free(casualties);
    fclose(input_fp);
    return NO_DATA;
  }

  /* calculate mean */

  avg_losses = sum_of_losses / number_of_battles_by_unit;
  double sum_squared_diff = 0.0;

  /* calculate sum of squared differences */

  for (int i = 0; i < number_of_battles_by_unit; i++) {
    double diff = casualties[i] - avg_losses;
    sum_squared_diff += diff * diff;
  }
  free(casualties);
  fclose(input_fp);
  return sqrt(sum_squared_diff / number_of_battles_by_unit);
} /* clone_unit_casualties_stdev() */

/*
 * cost_per_hour - Calculates average cost per hour
 * for a specific clone unit.
 */

double cost_per_hour(char *input_file, char *clone_unit){
  if ((clone_unit == NULL) || (strlen(clone_unit) == 0)) {
    return BAD_INPUT;
  }
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }
  if (empty_file_check(input_fp) == NO_DATA) {
    return NO_DATA;
  }
  int fields = 0;
  char planet[MAX_SIZE] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[MAX_SIZE] = "";
  char clone_unit_name[MAX_SIZE] = "";
  int r_forces = 0;
  int r_losses = 0;
  int s_forces = 0;
  int s_losses = 0;
  double sum_of_costs = 0.0;
  double total_duration_of_all_battles = 0.0;
  while (1) {
    fields = fscanf
        (input_fp,
         "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                      planet, &yy, &duration, &cost, &outcome,
                      jedi, clone_unit_name,
                      &r_forces, &r_losses, &s_forces, &s_losses);
    if ((strlen(planet) == MAX_NAME_LEN) ||
        (strlen(clone_unit_name) == MAX_NAME_LEN)) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != MAX_FIELDS) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (data_checker(planet, jedi, clone_unit_name, yy,
                     duration, cost, outcome, r_forces,
                     r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (strcmp(clone_unit_name, clone_unit) == 0) {
      sum_of_costs += cost;
      total_duration_of_all_battles += duration;
    }
  }
  fclose(input_fp);
  if (total_duration_of_all_battles <= 0.0) {
    return NO_DATA;
  }
  return sum_of_costs / total_duration_of_all_battles;
} /* cost_per_hour() */

/*
 * planetary_forces_ratio - calculates the ratio of Republic
 *  forces to Separatist forces for
 *  battles fought and won on a given planet.
 */

double planetary_forces_ratio(char *input_file, char *planet){
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }
  if (empty_file_check(input_fp) == NO_DATA) {
    return NO_DATA;
  }
  double number_of_battles = 0.0;
  double number_of_r_forces = 0.0;
  double number_of_s_forces = 0.0;
  int fields = 0;
  char planet_name[MAX_NAME_LEN] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[MAX_NAME_LEN] = "";
  char clone_unit_name[MAX_NAME_LEN] = "";
  int r_forces = 0;
  int r_losses = 0;
  int s_forces = 0;
  int s_losses = 0;
  if ((planet == NULL) ||
      (strlen(planet) == 0) || (strlen(planet) > MAX_NAME_LEN)) {
    fclose(input_fp);
    return BAD_INPUT;
  }
  while (1) {
    fields = fscanf
        (input_fp,
         "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                      planet_name, &yy, &duration, &cost, &outcome,
                      jedi, clone_unit_name,
                      &r_forces, &r_losses, &s_forces, &s_losses);
    if (strlen(planet) == MAX_NAME_LEN) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != MAX_FIELDS) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (data_checker(planet_name, jedi, clone_unit_name, yy,
                     duration, cost, outcome, r_forces,
                     r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if ((strcmp(planet, planet_name) == 0)) {
      number_of_battles++;
      if (outcome == 'W') {
        number_of_r_forces += r_forces;
        number_of_s_forces += s_forces;
      }
    }
  }
  fclose(input_fp);
  if (number_of_battles <= 0.0) {
    return NO_DATA;
  }

  /* returns ratio */

  return ((1 + number_of_r_forces) /
          ((number_of_battles) + (number_of_s_forces)));
} /* planetary_forces_ratio() */

/*
 * fallen_jedi_check - Determines if a Jedi is considered "fallen"
 *  in a battle based on cost, duration, and Republic forces lost.
 */

bool fallen_jedi_check(double cost, double duration, int r_forces,
                       int r_losses){
  int integer_cost = (int)floor(cost);
  bool cost_condition = false;
  if (((integer_cost % 66) == 0) || ((integer_cost % 1000) == 501)) {
    cost_condition = true;
  }
  if ((cost_condition == true) && (duration > 5555.27) &&
      (r_forces == r_losses)) {
    return true;
  }
  return false;
} /* fallen_jedi_check() */

/*
 * fallen_jedi_sacrifice - Calculates the effectiveness of
 * Jedi sacrifices across all battles in an input file.
 */

double fallen_jedi_sacrifice(char *input_file) {
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }
  if (empty_file_check(input_fp) == NO_DATA) {
    return NO_DATA;
  }
  double total_battles_where_jedi_fell = 0.0;
  double wins_jedi_fell = 0.0;
  int fields = 0;
  char planet[MAX_NAME_LEN] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[MAX_NAME_LEN] = "";
  char clone_unit_name[MAX_NAME_LEN] = "";
  int r_forces = 0;
  int r_losses = 0;
  int s_forces = 0;
  int s_losses = 0;
  while (1) {
    fields = fscanf
        (input_fp,
         "%[^,],%d,%lf,%lf,%c|\"%[^\"]\"&\"%[^\"]\"|%d+%d|%d+%d\n",
                planet, &yy, &duration, &cost, &outcome,
                jedi, clone_unit_name,
                &r_forces, &r_losses, &s_forces, &s_losses);
    if (strlen(planet) == MAX_NAME_LEN) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != MAX_FIELDS) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (data_checker(planet, jedi, clone_unit_name, yy,
                     duration, cost, outcome, r_forces,
                     r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fallen_jedi_check(cost, duration, r_forces,r_losses) == true) {
      total_battles_where_jedi_fell++;
      if (outcome == 'W') {
        wins_jedi_fell++;
      }
    }
  }
  if (total_battles_where_jedi_fell == 0) {
    fclose(input_fp);
    return NO_DATA;
  }
  fclose(input_fp);
  return (wins_jedi_fell / total_battles_where_jedi_fell);
} /* fallen_jedi_sacrifice() */

/*
 * truncate4 - Truncates a double value to 4 decimal places.
 */

double truncate4(double value) {
  return (value * 10000.0) / 10000.0;
} /* truncate4() */

/*
 * senate_report - Generates a Galactic Senate
 * expenditure report for a given year.
 */

int senate_report(char *input_file, char *output_file, int year) {
  if ((year < 19) || (year > 22)) {
    return BAD_INPUT;
  }
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }
  if (empty_file_check(input_fp) == NO_DATA) {
    return NO_DATA;
  }
  int fields = 0;
  char planet_name[MAX_SIZE] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[MAX_SIZE] = "";
  char clone_unit_name[MAX_SIZE] = "";
  int r_forces = 0;
  int r_losses = 0;
  int s_forces = 0;
  int s_losses = 0;
  int total_battles = 0;
  int total_battles_won = 0;
  double total_expenditure = 0.0;
  int total_republic_forces = 0;
  double total_win_cost = 0.0;
  while (1) {
    fields = fscanf
        (input_fp,
         "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                      planet_name, &yy, &duration, &cost, &outcome,
                      jedi, clone_unit_name,
                      &r_forces, &r_losses, &s_forces, &s_losses);
    if (strlen(planet_name) == MAX_NAME_LEN) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != MAX_FIELDS) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (data_checker(planet_name, jedi, clone_unit_name, yy,
                     duration, cost, outcome, r_forces, r_losses,
                     s_forces, s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (year == yy) {
      total_battles++;
      total_expenditure += cost;
      total_republic_forces += r_forces;
      if (outcome == 'W') {
        total_battles_won++;
        total_win_cost += cost;
      }
    }
  }
  if ((total_battles == 0) || (total_republic_forces == 0)) {
    return NO_DATA;
  }
  double avg_cost_per_battle =
      truncate4(total_expenditure / total_battles);
  double avg_cost_per_soldier =
      truncate4(total_expenditure / total_republic_forces);
  double victory_cost_percent =
      truncate4((total_win_cost / total_expenditure) * 100.0);
  FILE *output_fp = fopen(output_file, "w");
  if (output_fp == NULL) {
    fclose(input_fp);
    return FILE_WRITE_ERR;
  }
  fclose(input_fp);
  char *header1 = "==================================================";
  char *header2 = " == GALACTIC SENATE DEFENSE EXPENDITURE REPORT ==";
  char *header3 = " ============= FOR SENATE EYES ONLY =============";
  fprintf(output_fp, "%s\n%s\n%s\n%s\n\n", header1, header2, header3,
          header1);
  fprintf(output_fp, "Total Battles Funded: %d\n", total_battles);

  /* Print total expenditure directly and measure length for dashes */

  int chars_written = fprintf(output_fp,
                              "Total Expenditure: %.4f Galactic Credits\n",
                              truncate4(total_expenditure));
  for (int i = 0; i < chars_written - 1; i++) {
    fprintf(output_fp, "-");
  }
  fprintf(output_fp, "\n");
  fprintf(output_fp,
          "Average Cost per Battle: %.4f Galactic Credits\n",
          avg_cost_per_battle);
  fprintf(output_fp,
          "Average Cost per Soldier: %.4f Galactic Credits\n",
          avg_cost_per_soldier);
  fprintf(output_fp,
          "Victory Cost: %.4f%%\n", victory_cost_percent);
  fprintf(output_fp, "\nFor the Republic!\n");
  fclose(output_fp);
  return total_battles_won;
} /* senate_report() */

/*
 * jedi_council_report - Generates a report
  *  of Jedi victories and sacrifices.
 */

int jedi_council_report(char *input_file, char *output_file) {
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }
  if (empty_file_check(input_fp) == NO_DATA) {
    return NO_DATA;
  }
  int fields = 0;
  char planet_name[MAX_SIZE] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[MAX_SIZE] = "";
  char clone_unit_name[MAX_SIZE] = "";
  int r_forces = 0;
  int r_losses = 0;
  int s_forces = 0;
  int s_losses = 0;
  char current_jedi[MAX_NAME_LEN] = "";
  int current_victories = 0;
  char most_victorious_jedi[MAX_NAME_LEN] = "";
  int max_victories = 0;
  int fallen_victories = 0;
  typedef struct {
      char jedi[MAX_NAME_LEN];
      char clone_unit[MAX_NAME_LEN];
      char planet[MAX_NAME_LEN];
  } fallen_t;
  fallen_t fallen_heroes[MAX_SIZE];
  int fallen_count = 0;
  while (1) {
    fields = fscanf
        (input_fp,
         "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                      planet_name, &yy, &duration, &cost, &outcome,
                      jedi, clone_unit_name,
                      &r_forces, &r_losses, &s_forces, &s_losses);
    if (strlen(planet_name) == MAX_NAME_LEN) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != MAX_FIELDS) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (data_checker(planet_name, jedi, clone_unit_name, yy,
                     duration, cost, outcome, r_forces,
                     r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (strcmp(jedi, current_jedi) != 0) {
      if (current_victories > max_victories) {
          max_victories = current_victories;
          strcpy(most_victorious_jedi, current_jedi);
      }
      strcpy(current_jedi, jedi);
      current_victories = 0;
    }
    if (outcome == 'W') {
      current_victories++;
    }
    if (fallen_jedi_check(cost, duration, r_forces, r_losses)) {
      strcpy(fallen_heroes[fallen_count].jedi, jedi);
      strcpy(fallen_heroes[fallen_count].clone_unit, clone_unit_name);
      strcpy(fallen_heroes[fallen_count].planet, planet_name);
      fallen_count++;
    }
  }
  if (current_victories > max_victories) {
      max_victories = current_victories;
      strcpy(most_victorious_jedi, current_jedi);
  }
  FILE *output_fp = fopen(output_file, "w");
  if (output_fp == NULL) {
    fclose(input_fp);
    return FILE_WRITE_ERR;
  }
  fclose(input_fp);
  char *header1 = "===========================================";
  char *header2 = " == JEDI HIGH COUNCIL TACTICAL BRIEFING ==";
  char *header3 = " ======= FOR JEDI MASTER EYES ONLY =======";
  fprintf(output_fp, "%s\n%s\n%s\n%s\n\n", header1, header2, header3,
          header1);
  fprintf(output_fp,
          "Most Victorious Jedi General: %s with %d victories\n",
          most_victorious_jedi, max_victories);
  fprintf(output_fp, "Jedi Casualties: %d\n", fallen_count);
  if (fallen_count > 0) {
      double sacrifice_effectiveness = fallen_jedi_sacrifice(input_file);
      fallen_victories = (int) (sacrifice_effectiveness * fallen_count);
      fprintf(output_fp, "Jedi Sacrifice Effectiveness: %.4f%%\n\n",
              (100 * sacrifice_effectiveness));
  }
  else if (fallen_count == 0) {
    fprintf(output_fp, "Jedi Sacrifice Effectiveness: N/A\n");
    return fallen_victories;
  }
  if (fallen_count > 0) {
    char *fallen_header = "Fallen Heroes and Lost Units:";
    fprintf(output_fp, "%s\n", fallen_header);
    fprintf(output_fp, "-----------------------------\n");
      for (int i = 0; i < fallen_count; i++) {
        fprintf(output_fp, "%s fell alongside their %s unit on %s\n",
                fallen_heroes[i].jedi,
                fallen_heroes[i].clone_unit,
                fallen_heroes[i].planet);
      }
  }
  fprintf
      (output_fp,
       "\nMay the Force guide our strategy to bring peace to the galaxy!\n");
  fclose(output_fp);
  return fallen_victories;
} /* jedi_council_report() */
