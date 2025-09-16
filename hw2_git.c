//
// Created by adity on 9/10/2025.
//

#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_NAME_LEN (50)

#define BAD_DATA (-1)
#define BAD_INPUT (-2)
#define FILE_READ_ERR (-3)
#define FILE_WRITE_ERR (-4)
#define NO_DATA (-5)


double casualty_ratio(char *input_file, int year); //check
double cost_per_hour(char *input_file, char *clone_unit); //check
double planetary_forces_ratio(char *input_file, char *planet); //check
double clone_unit_casualties_stdev(char *input_file, char *clone_unit); //check
double fallen_jedi_sacrifice(char *input_file); //check
int senate_report(char *input_file, char *output_file, int year); //
int jedi_council_report(char *input_file, char *output_file);

//checks the validity of each field and returns bad data if any of them are wrong
bool data_checker (char *planet, char *jedi, char *clone_unit,
  int year, double duration, double cost, char outcome,
  int r_forces, int r_losses, int s_forces, int s_losses) {
  // planet, jedi, clone_unit must not be empty
  if (planet == NULL || strlen(planet) == 0 || strlen(planet) > MAX_NAME_LEN) {
    return false;
  }
  if (jedi == NULL || strlen(jedi) == 0 || strlen(jedi) > MAX_NAME_LEN) {
    return false;
  }
  if (clone_unit == NULL || strlen(clone_unit) == 0 || strlen(clone_unit) > MAX_NAME_LEN) {
    return false;
  }
  if ((year < 19) || (year > 22)) {
    return false;
  }
  if ((duration <= 0.0) || (duration >= 8760.0)) {
    return false;
  }
  if (cost <= 0.0) {
    return false;
  }
  if ((outcome != 'W') && (outcome != 'L') && (outcome != 'T')) {
    return false;
  }
  if ((r_forces <= 0) || (s_forces <= 0)) {
    return false;
  }
  if ((r_losses < 0) || (r_losses > r_forces)) {
    return false;
  }
  if ((s_losses < 0) || (s_losses > s_forces)) {
    return false;
  }
  return true;
}



double casualty_ratio(char *input_file, int year) {
  if ((year < 19) || (year > 22)) {
    return BAD_INPUT;
  }
  char planet[1000] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[1000] = "";
  char clone_unit[1000] = "";
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

  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }

  int first_char;
  while ((first_char = fgetc(input_fp)) != EOF) {
    if (!isspace(first_char)) {
      break;
    }
  }

  if (first_char == EOF) {
    fclose(input_fp);
    return NO_DATA; // File is empty or only whitespace
  }

  rewind(input_fp);


  while (1) {
    fields = fscanf(input_fp, "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
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
    if (fields != 11) {
      fclose(input_fp);
      return BAD_DATA;
    }
    //should check for bad data here
    if (data_checker(planet, jedi, clone_unit,yy, duration, cost, outcome, r_forces, r_losses, s_forces, s_losses) == false) {
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
  republic_cr = (double)(1 + r_sum_of_forces_lost_in_year) / (number_of_battles_in_year + r_sum_of_forces_deployed_in_year);
  separatist_cr = (double)(1 + s_sum_of_forces_lost_in_year) / (number_of_battles_in_year + s_sum_of_forces_deployed_in_year);

  if (separatist_cr == 0) {
    return BAD_INPUT;
  }

  return republic_cr/separatist_cr;
}

/// HW FUNCTIONS
/* returns casualty_ratio */

double clone_unit_casualties_stdev_1(char *input_file, char *clone_unit){
  if (clone_unit == NULL || strlen(clone_unit) == 0) {
    return BAD_INPUT;
  }

  double stdev = 0.0;
  double sum_of_losses = 0.0;
  double avg_losses = 0.0;
  double number_of_battles_by_unit = 0.0;

  char planet[1000] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[1000] = "";
  char clone_unit_name[1000] = "";
  int r_forces = 0;
  int r_losses = 0;
  int s_forces = 0;
  int s_losses = 0;

  int fields = 0;

  FILE *input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }

  int first_char;
  while ((first_char = fgetc(input_fp)) != EOF) {
    if (!isspace(first_char)) {
      break;
    }
  }
  if (first_char == EOF) {
    fclose(input_fp);
    return NO_DATA; // File is empty or only whitespace
  }
  rewind(input_fp);

  while (1) {
    fields = fscanf(input_fp, "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
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
    if (fields != 11) {
      fclose(input_fp);
      return BAD_DATA;
    }
    //should check for bad data here
    if (data_checker(planet, jedi, clone_unit_name,yy, duration, cost, outcome, r_forces, r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (strcmp(clone_unit,clone_unit_name) == 0) {
      number_of_battles_by_unit++;
      sum_of_losses += r_losses;
    }
  }
  if (number_of_battles_by_unit == 0) {
    fclose(input_fp);
    return NO_DATA;
  }
  if (sum_of_losses == 0) {
    fclose(input_fp);
    return NO_DATA;
  }

  avg_losses = sum_of_losses / number_of_battles_by_unit; //avg found
  rewind(input_fp);
  while (1) {
    fields = fscanf(input_fp, "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
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
    if (fields != 11) {
      fclose(input_fp);
      return BAD_DATA;
    }
    //should check for bad data here
    if (data_checker(planet, jedi, clone_unit_name,yy, duration, cost, outcome, r_forces, r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (strcmp(clone_unit, clone_unit_name) == 0) {
      stdev += pow((r_losses - avg_losses),2);
    }
  }
  fclose(input_fp);
  stdev = sqrt(stdev / number_of_battles_by_unit);
  return stdev;
}

/** Compare strings without case or space check
*/
// Single function to compare strings ignoring case and spaces
int compare_ignore_case_space(const char *str1, const char *str2) {
  int i = 0, j = 0;

  while (str1[i] != '\0' && str2[j] != '\0') {
    // Skip spaces in both strings
    while (str1[i] == ' ') i++;
    while (str2[j] == ' ') j++;

    // If either string ends after skipping spaces, break
    if (str1[i] == '\0' || str2[j] == '\0') {
      break;
    }

    // Compare characters case-insensitively
    if (tolower((unsigned char)str1[i]) != tolower((unsigned char)str2[j])) {
      return tolower((unsigned char)str1[i]) - tolower((unsigned char)str2[j]);
    }

    i++;
    j++;
  }

  // Skip any remaining spaces in both strings
  while (str1[i] == ' ') i++;
  while (str2[j] == ' ') j++;

  // Check if both strings have reached the end
  if (str1[i] == '\0' && str2[j] == '\0') {
    return 0;  // Strings are equal
  } else if (str1[i] == '\0') {
    return -1; // str1 is shorter
  } else {
    return 1;  // str2 is shorter
  }
}
/// HW FUNCTIONS
/* returns casualty_ratio */

double clone_unit_casualties_stdev(char *input_file, char *clone_unit){
  printf("\n clone_unit_casualties_stdev======file name:%s, clone_unit:%s", input_file, clone_unit);
  if (clone_unit == NULL || strlen(clone_unit) == 0) {
    return BAD_INPUT;
  }

  double stdev = 0.0;
  double sum_of_losses = 0.0;
  double avg_losses = 0.0;
  double number_of_battles_by_unit = 0.0;

  char planet[1000] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[1000] = "";
  char clone_unit_name[1000] = "";
  int r_forces = 0;
  int r_losses = 0;
  int s_forces = 0;
  int s_losses = 0;

  int fields = 0;
  // to hold r losses
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

  int first_char;
  while ((first_char = fgetc(input_fp)) != EOF) {
    if (!isspace(first_char)) {
      break;
    }
  }

  if (first_char == EOF) {
    fclose(input_fp);
    return NO_DATA; // File is empty or only whitespace
  }

  rewind(input_fp);

  while (1) {
    fields = fscanf(input_fp, "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                planet, &yy, &duration, &cost, &outcome,
                jedi, clone_unit,
                &r_forces, &r_losses, &s_forces, &s_losses);
    if (strlen(planet) == MAX_NAME_LEN || strlen(clone_unit) == MAX_NAME_LEN) {
      fclose(input_fp);
      printf("\n 1......bad data   planet:%s, clone unit:%s", planet, clone_unit );
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != 11) {
      fclose(input_fp);
      return BAD_DATA;
    }
    //should check for bad data here
    if (data_checker(planet, jedi, clone_unit_name,yy, duration, cost, outcome, r_forces, r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      printf("\n 2......bad data   planet:%s, clone unit:%s", planet, clone_unit );
      return BAD_DATA;
    }
    if (compare_ignore_case_space(clone_unit,clone_unit_name) == 1) {

      if (size >= capacity) {
        capacity *= 2;
        double *temp = (double *)realloc(casualties, capacity * sizeof(double));
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
  // If no battles found for this clone unit
  if (number_of_battles_by_unit == 0) {
    free(casualties);
    return 0.0;
  }

  // Calculate mean
  avg_losses = sum_of_losses / number_of_battles_by_unit; //avg found
  double sum_squared_diff = 0.0;
  // Calculate sum of squared differences
  for (int i = 0; i < number_of_battles_by_unit; i++) {
    double diff = casualties[i] - avg_losses;
    sum_squared_diff += diff * diff;
  }

  free(casualties);
// Calculate and return standard deviation
  double result = sqrt(sum_squared_diff / number_of_battles_by_unit);
  printf("The std is:%lf", result);
  return result;
}

/* Calculate cost per hour
*/
double cost_per_hour(char *input_file, char *clone_unit){

  printf("File Name:%s, Clone Unit:%s", input_file, clone_unit);
  if (clone_unit == NULL || strlen(clone_unit) == 0) {
    return BAD_INPUT;
  }
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }

  int first_char;
  while ((first_char = fgetc(input_fp)) != EOF) {
    if (!isspace(first_char)) {
      break;
    }
  }

  if (first_char == EOF) {
    fclose(input_fp);
    return NO_DATA; // File is empty or only whitespace
  }

  rewind(input_fp);

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
  double sum_of_costs = 0.0;
  double total_duration_of_all_battles = 0.0;

  while (1) {
    fields = fscanf(input_fp, "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                      planet, &yy, &duration, &cost, &outcome,
                      jedi, clone_unit,
                      &r_forces, &r_losses, &s_forces, &s_losses);
    // Check for truncated names (assuming max length of 50)
    if (strlen(planet) == MAX_NAME_LEN || (strlen(clone_unit) == MAX_NAME_LEN)) {
      // Name was truncated, handle as error or warning
      printf("/n 2..Clone Unit:%s, PLANET:%s\n", clone_unit, planet);
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != 11) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (data_checker(planet, jedi, clone_unit,yy, duration, cost, outcome, r_forces, r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      printf("/n 3.....Clone Unit:%s, PLANET:%s\n", clone_unit, planet);
      return BAD_DATA;
    }
    if (compare_ignore_case_space(clone_unit_name, clone_unit) == 1) {
      sum_of_costs += cost;
      total_duration_of_all_battles += duration;
    }
  }
  fclose(input_fp);
  if (total_duration_of_all_battles <= 0.0) {
    return NO_DATA;
  }
  //COMMENT OUT
  printf("costs per hour = %lf\n", sum_of_costs/total_duration_of_all_battles);
  return sum_of_costs / total_duration_of_all_battles; //cost_per_hour
}























double cost_per_hour_1(char *input_file, char *clone_unit){
  if (clone_unit == NULL || strlen(clone_unit) == 0) {
    return BAD_INPUT;
  }
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }

  int first_char;
  while ((first_char = fgetc(input_fp)) != EOF) {
    if (!isspace(first_char)) {
      break;
    }
  }

  if (first_char == EOF) {
    fclose(input_fp);
    return NO_DATA; // File is empty or only whitespace
  }

  rewind(input_fp);

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
  double sum_of_costs = 0.0;
  double total_duration_of_all_battles = 0.0;

  while (1) {
    fields = fscanf(input_fp, "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                      planet, &yy, &duration, &cost, &outcome,
                      jedi, clone_unit,
                      &r_forces, &r_losses, &s_forces, &s_losses);
    // Check for truncated names (assuming max length of 50)
    if (strlen(planet) == MAX_NAME_LEN) {
      // Name was truncated, handle as error or warning
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != 11) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (data_checker(planet, jedi, clone_unit,yy, duration, cost, outcome, r_forces, r_losses, s_forces, s_losses) == false) {
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
    if (sum_of_costs > 0.0) {
      return sum_of_costs;
    }
    return NO_DATA;
  }
  //COMMENT OUT
  printf("costs per hour = %lf\n", sum_of_costs/total_duration_of_all_battles);
  return sum_of_costs / total_duration_of_all_battles; //cost_per_hour
}

double planetary_forces_ratio(char *input_file, char *planet){
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }

  int first_char;
  while ((first_char = fgetc(input_fp)) != EOF) {
    if (!isspace(first_char)) {
      break;
    }
  }

  if (first_char == EOF) {
    fclose(input_fp);
    return NO_DATA; // File is empty or only whitespace
  }

  rewind(input_fp);

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

  if (planet == NULL || strlen(planet) == 0 || strlen(planet) > MAX_NAME_LEN) {
    fclose(input_fp);
    return BAD_INPUT;
  }
  while (1) {
    fields = fscanf(input_fp, "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                      planet_name, &yy, &duration, &cost, &outcome,
                      jedi, clone_unit_name,
                      &r_forces, &r_losses, &s_forces, &s_losses);
    // Check for truncated names (assuming max length of 50)
    if (strlen(planet) == MAX_NAME_LEN) {
      // Name was truncated, handle as error or warning
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != 11) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (data_checker(planet_name, jedi, clone_unit_name,yy, duration, cost, outcome, r_forces, r_losses, s_forces, s_losses) == false) {
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
  return ((1 + number_of_r_forces) / ((number_of_battles) + (number_of_s_forces))); //ratio returned
}

bool fallen_jedi_check(double cost, double duration, int r_forces, int r_losses){
  int integer_cost = (int)floor(cost);
  bool cost_condition = false;
  if (((integer_cost % 66) == 0) || ((integer_cost % 1000) == 501)){
    cost_condition = true;
  }
  if ((cost_condition == true) && (duration > 5555.27) && (r_forces == r_losses)) {
    return true;
  }
  return false;
}

double fallen_jedi_sacrifice(char *input_file) {
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }

  int first_char;
  while ((first_char = fgetc(input_fp)) != EOF) {
    if (!isspace(first_char)) {
      break;
    }
  }

  if (first_char == EOF) {
    fclose(input_fp);
    return NO_DATA; // File is empty or only whitespace
  }

  rewind(input_fp);

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
    fields = fscanf(input_fp, "%[^,],%d,%lf,%lf,%c|\"%[^\"]\"&\"%[^\"]\"|%d+%d|%d+%d\n",
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
    if (fields != 11) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (data_checker(planet, jedi, clone_unit_name,yy, duration, cost, outcome, r_forces, r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fallen_jedi_check(cost,duration,r_forces,r_losses) == true) {
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
  return (wins_jedi_fell/ total_battles_where_jedi_fell);
}

double truncate4(double value) {
  return (value * 10000.0) / 10000.0;
}

///TEST THIS OUT LATER FOR LESS CODE
int file_empty_check(FILE *input_fp) {
  int first_char;
  while ((first_char = fgetc(input_fp)) != EOF) {
    if (!isspace(first_char)) {
      break;
    }
  }
  if (first_char == EOF) {
    fclose(input_fp);
    return NO_DATA; // File is empty or only whitespace
  }
  rewind(input_fp);
  return 0; //success
}

int senate_report(char *input_file, char *output_file, int year) {
  if ((year < 19) || (year > 22)) {
    return BAD_INPUT;
  }
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }

  int first_char;
  while ((first_char = fgetc(input_fp)) != EOF) {
    if (!isspace(first_char)) {
      break;
    }
  }
  if (first_char == EOF) {
    fclose(input_fp);
    return NO_DATA; // File is empty or only whitespace
  }
  rewind(input_fp);

  int fields = 0;

  char planet_name[1000] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[1000] = "";
  char clone_unit_name[1000] = "";
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
    fields = fscanf(input_fp, "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                      planet_name, &yy, &duration, &cost, &outcome,
                      jedi, clone_unit_name,
                      &r_forces, &r_losses, &s_forces, &s_losses);
    // Check for truncated names (assuming max length of 50)
    if (strlen(planet_name) == MAX_NAME_LEN) {
      // Name was truncated, handle as error or warning
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != 11) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (data_checker(planet_name, jedi, clone_unit_name,yy, duration, cost, outcome, r_forces, r_losses, s_forces, s_losses) == false) {
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
  double avg_cost_per_battle = truncate4(total_expenditure / total_battles);
  double avg_cost_per_soldier = truncate4(total_expenditure / total_republic_forces);
  double victory_cost_percent = truncate4((total_win_cost / total_expenditure) * 100.0);
  FILE *output_fp = fopen(output_file, "w");
  if (output_fp == NULL) {
    fclose(input_fp);
    return FILE_WRITE_ERR;
  }
  fclose(input_fp);
  char *header1 = "==================================================";
  char *header2 = " == GALACTIC SENATE DEFENSE EXPENDITURE REPORT ==";
  char *header3 = " ============= FOR SENATE EYES ONLY =============";
  fprintf(output_fp, "%s\n%s\n%s\n%s\n\n", header1, header2, header3, header1);
  fprintf(output_fp, "Total Battles Funded: %d\n", total_battles);
  // Print total expenditure directly and measure length for dashes
  int chars_written = fprintf(output_fp, "Total Expenditure: %.4f Galactic Credits\n", truncate4(total_expenditure));
  for (int i = 0; i < chars_written - 1; i++) {
    fprintf(output_fp, "-");
  }
  fprintf(output_fp, "\n");
  fprintf(output_fp, "Average Cost per Battle: %.4f Galactic Credits\n", avg_cost_per_battle);
  fprintf(output_fp, "Average Cost per Soldier: %.4f Galactic Credits\n", avg_cost_per_soldier);
  fprintf(output_fp, "Victory Cost: %.4f%%\n", victory_cost_percent);
  fprintf(output_fp, "\nFor the Republic!\n");
  fclose(output_fp);
  return total_battles_won;
}


int jedi_council_report(char *input_file, char *output_file) {
  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }
  int first_char;
  while ((first_char = fgetc(input_fp)) != EOF) {
    if (!isspace(first_char)) {
      break;
    }
  }
  if (first_char == EOF) {
    fclose(input_fp);
    return NO_DATA; // File is empty or only whitespace
  }
  rewind(input_fp);

  int fields = 0;

  char planet_name[1000] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[1000] = "";
  char clone_unit_name[1000] = "";
  int r_forces = 0;
  int r_losses = 0;
  int s_forces = 0;
  int s_losses = 0;

  // Track Jedi victories
  char current_jedi[MAX_NAME_LEN] = "";
  int current_victories = 0;
  char most_victorious_jedi[MAX_NAME_LEN] = "";
  int max_victories = 0;

  int fallen_victories = 0;

  // Track fallen Jedi
  typedef struct {
      char jedi[MAX_NAME_LEN];
      char clone_unit[MAX_NAME_LEN];
      char planet[MAX_NAME_LEN];
  } fallen_t;

  fallen_t fallen_heroes[1000]; // arbitrary limit
  int fallen_count = 0;

  while (1) {
    fields = fscanf(input_fp, "%49[^,],%d,%lf,%lf,%c|\"%49[^\"]\"&\"%49[^\"]\"|%d+%d|%d+%d\n",
                      planet_name, &yy, &duration, &cost, &outcome,
                      jedi, clone_unit_name,
                      &r_forces, &r_losses, &s_forces, &s_losses);
    // Check for truncated names (assuming max length of 50)
    if (strlen(planet_name) == MAX_NAME_LEN) {
      // Name was truncated, handle as error or warning
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
    }
    if (fields != 11) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (data_checker(planet_name, jedi, clone_unit_name,yy, duration, cost, outcome, r_forces, r_losses, s_forces, s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    // Count victories per Jedi
    if (strcmp(jedi, current_jedi) != 0) {
      // New Jedi group, check previous
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
    // Check fallen Jedi
    if (fallen_jedi_check(cost, duration, r_forces, r_losses)) {
      strcpy(fallen_heroes[fallen_count].jedi, jedi);
      strcpy(fallen_heroes[fallen_count].clone_unit, clone_unit_name);
      strcpy(fallen_heroes[fallen_count].planet, planet_name);
      fallen_count++;
    }
  }
  // Check last Jedi group
  if (current_victories > max_victories) {
      max_victories = current_victories;
      strcpy(most_victorious_jedi, current_jedi);
  }
  // Open output
  FILE *output_fp = fopen(output_file, "w");
  if (output_fp == NULL) {
    fclose(input_fp);
    return FILE_WRITE_ERR;
  }
  fclose(input_fp);
  char *header1 = "===========================================";
  char *header2 = " == JEDI HIGH COUNCIL TACTICAL BRIEFING ==";
  char *header3 = " ======= FOR JEDI MASTER EYES ONLY =======";
  fprintf(output_fp, "%s\n%s\n%s\n%s\n\n", header1, header2, header3, header1);
  fprintf(output_fp, "Most Victorious Jedi General: %s with %d victories\n",
          most_victorious_jedi, max_victories);
  fprintf(output_fp, "Jedi Casualties: %d\n", fallen_count);
  if (fallen_count > 0) {
      double sacrifice_effectiveness = fallen_jedi_sacrifice(input_file);
      fallen_victories = (int) (sacrifice_effectiveness * fallen_count);
      fprintf(output_fp, "Jedi Sacrifice Effectiveness: %.4f%%\n", sacrifice_effectiveness);
  } else if (fallen_count == 0) {
      fprintf(output_fp, "Jedi Sacrifice Effectiveness: N/A\n");
      return fallen_victories; //no fallen_victories
  }
  // fallen heroes section
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
  fprintf(output_fp, "\nMay the Force guide our strategy to bring peace to the galaxy!\n");
  fclose(output_fp);

  return fallen_victories;
}

//  int main (){
//    // read_battle_records("C:/Users/adity/OneDrive/Desktop/clone_wars.txt");
//  // printf("%lf\n", casualty_ratio("C:/Users/adity/OneDrive/Desktop/clone_wars.txt",22));
//   casualty_ratio("C:/Users/adity/OneDrive/Desktop/clone_wars.txt",19);
//    // cost_per_hour("C:/Users/adity/OneDrive/Desktop/clone_wars.txt", "21st Nova Corps");
//    // cost_per_hour("C:/Users/adity/OneDrive/Desktop/clone_wars.txt", "212th Attack Battalion");
//    // cost_per_hour("C:/Users/adity/OneDrive/Desktop/clone_wars.txt", "501st Legion");
//   return 0;
// }

