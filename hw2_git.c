//
// Created by adity on 9/10/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_NAME_LEN (50)

#define BAD_DATA (-1)
#define BAD_INPUT (-2)
#define FILE_READ_ERR (-3)
#define FILE_WRITE_ERR (-4)
#define NO_DATA (-5)


double casualty_ratio(char *input_file, int year);
double cost_per_hour(char *input_file, char *clone_unit);
double planetary_forces_ratio(char *input_file, char *planet);
double clone_unit_casualties_stdev(char *input_file, char *clone_unit);
double fallen_jedi_sacrifice(char *input_file);
int senate_report(char *input_file, char *output_file, int year);
int jedi_council_report(char *input_file, char *output_file);

//checks the validity of each field and returns bad data if any of them are wrong
bool data_checker(int fields, int year, double duration, double cost, char outcome,
  int r_forces, int r_losses, int s_forces, int s_losses) {

  if ((fields != 11) && (fields != EOF)) {
    return false;
  }
  if ((year < 19) || (year > 22)) {
    return false;
  }
  if ((duration <= 0) || (duration >= 8760)) {
    return false;
  }
  if (cost <= 0) {
    return false;
  }
  if ((r_forces < 0) || (s_forces < 0)) {
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

// void read_battle_records(const char *filename) {
//   FILE *file = fopen(filename, "r");
//   if (file == NULL) {
//     perror("Error opening file");
//     return;
//   }
//
//   char planet[MAX_NAME_LEN];
//   int yy;
//   double duration = 0.0;
//   double cost = 0.0;
//   char outcome;
//   char jedi[MAX_NAME_LEN];
//   char clone_unit[MAX_NAME_LEN];
//   int r_forces, r_losses, s_forces, s_losses;
//
//   while (fscanf(file,
//                 "%[^,],%d,%lf,%lf,%c|\"%[^\"]\"&\"%[^\"]\"|%d+%d|%d+%d\n",
//                 planet, &yy, &duration, &cost, &outcome,
//                 jedi, clone_unit,
//                 &r_forces, &r_losses, &s_forces, &s_losses) == 11) {
//
//     // Process the record here (print for demonstration)
//     printf("Planet: %s\n", planet);
//     printf("Year: %d\n", yy);
//     printf("Duration: %.2f hours\n", duration);
//     printf("Cost: %.2f credits\n", cost);
//     printf("Outcome: %c\n", outcome);
//     printf("Jedi: %s\n", jedi);
//     printf("Clone Unit: %s\n", clone_unit);
//     printf("Republic Forces: %d, Losses: %d\n", r_forces, r_losses);
//     printf("Separatist Forces: %d, Losses: %d\n", s_forces, s_losses);
//     printf("----------------------------------------\n");
//   }
//   fclose(file);
// }

double casualty_ratio(char *input_file, int year) {
  char planet[MAX_NAME_LEN] = "";
  int yy = 0;
  double duration = 0.0;
  double cost = 0.0;
  char outcome = '0';
  char jedi[MAX_NAME_LEN] = "";
  char clone_unit[MAX_NAME_LEN] = "";
  int r_forces = 0;
  int r_losses = 0;
  int s_forces = 0;
  int s_losses = 0;

  double republic_cr = 0;
  double separatist_cr = 0;
  int number_of_battles_in_year = 0;
  int r_sum_of_forces_deployed_in_year = 0;
  int r_sum_of_forces_lost_in_year = 0;
  int s_sum_of_forces_deployed_in_year = 0;
  int s_sum_of_forces_lost_in_year = 0;

  int fields = 0;

  FILE *input_fp = NULL;
  input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    perror("Error opening file");
    return FILE_READ_ERR;
  }
  while (1) {
    fields = fscanf(input_fp, "%[^,],%d,%lf,%lf,%c|\"%[^\"]\"&\"%[^\"]\"|%d+%d|%d+%d\n",
                planet, &yy, &duration, &cost, &outcome,
                jedi, clone_unit,
                &r_forces, &r_losses, &s_forces, &s_losses);
    //should check for bad data here
    if (data_checker(fields,yy,duration,cost,outcome,r_forces,r_losses,s_forces,s_losses) == false) {
      fclose(input_fp);
      return BAD_DATA;
    }
    if (fields == EOF) {
      break;
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
    printf("no data"); // comment out
    return NO_DATA;
  }
  republic_cr = (double)(1 + r_sum_of_forces_lost_in_year) / (number_of_battles_in_year + r_sum_of_forces_deployed_in_year);
  separatist_cr = (double)(1 + s_sum_of_forces_lost_in_year) / (number_of_battles_in_year + s_sum_of_forces_deployed_in_year);

  if (separatist_cr == 0) {
    return BAD_DATA;
  }
  
  printf("Republic Forces: %.2f\n", republic_cr);
  printf("Separatist Forces: %.2f\n", separatist_cr);

  printf("%f\n", (republic_cr / separatist_cr));
  return republic_cr/separatist_cr;
}

/// HW FUNCTIONS
/* returns casualty_ratio */

double clone_unit_casualties_stdev(char *a, char *b){
  return 0.0;
}
double cost_per_hour(char *a, char *b){
  return 0.0;
}
double planetary_forces_ratio(char *a, char *b){
  return 0.0;
}
double fallen_jedi_sacrifce(char *a){
  return 0.0;
}
int senate_report(char *a, char *b, int c){
  return 0;
}
int jedi_council_report(char *a, char *b){
  return 0;
}

// int main (){
//  // read_battle_records("C:/Users/adity/OneDrive/Desktop/clone_wars.txt");
//   casualty_ratio("C:/Users/adity/OneDrive/Desktop/clone_wars.txt",22);
//   return 0;
// }
//




