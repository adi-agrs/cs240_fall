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


int read_battle(FILE *fp, void *out_record) {
  typedef struct {
    char planet[MAX_NAME_LEN];
    int year;
    double duration;
    double cost;
    char outcome[5];
    char jedi[MAX_NAME_LEN];
    char clone_unit[MAX_NAME_LEN];
    int r_forces;
    int r_losses;
    int s_forces;
    int s_losses;
  } t_battle_record;
  t_battle_record *br = (t_battle_record *) out_record;
  return (fscanf(fp, "%[^,],%d,%lf,%lf,%[^|]|\"%[^\"]\"&\"%[^\"]\"|%d+%d|%d+%d\n",
         br->planet, &br->year, &br->duration, &br->cost,br->outcome, br->jedi,
         br->clone_unit, &br->r_forces, &br->r_losses, &br->s_forces, &br->s_losses));
}

double calculate_causalty_rate_from_year(char *input_file, int year, char *side){
  double causalty_rate = 0;
  int number_of_battles_in_year = 0;
  int sum_of_forces_deployed_in_year = 0;
  int sum_of_forces_lost_in_year = 0;

  typedef struct {
    char planet[MAX_NAME_LEN];
    int year;
    double duration;
    double cost;
    char outcome[5];
    char jedi[MAX_NAME_LEN];
    char clone_unit[MAX_NAME_LEN];
    int r_forces;
    int r_losses;
    int s_forces;
    int s_losses;
  } t_battle_record;

  FILE *input_fp;
  input_fp = fopen(input_file, "r");
  char buffer[1000];
  t_battle_record br;
  while (read_battle(input_fp, &br) == 11) { // all 11 items must be parsed
    if (br.year == year) {
      number_of_battles_in_year++;
      if (strcmp(side, "Republic") == 0) {
          sum_of_forces_deployed_in_year += br.r_forces;
          sum_of_forces_lost_in_year += br.r_losses;
      } else if (strcmp(side, "Separatist") == 0) {
          sum_of_forces_deployed_in_year += br.s_forces;
          sum_of_forces_lost_in_year += br.s_losses;
      }
    }
  }
  fclose(input_fp);
  if (number_of_battles_in_year <= 0) {
      return NO_DATA;
  }

  return ((1 + (double) sum_of_forces_lost_in_year) /
          ((double) number_of_battles_in_year + (double) sum_of_forces_deployed_in_year));
}

/// HW FUNCTIONS
/* returns casualty_ratio */
double casualty_ratio(char *input_file, int year){
  double casualty_ratio;
  double republic_cr = 0;
  double seperatist_cr = 0;
  //handle bad input in this function
  //write errors impossible
  if ((calculate_causalty_rate_from_year(input_file, year, "Republic") == NO_DATA)
      || (calculate_causalty_rate_from_year(input_file, year, "Separatist") == NO_DATA)) {
    return NO_DATA;
  }
  else if ((calculate_causalty_rate_from_year(input_file, year, "Republic") == BAD_DATA)
           || (calculate_causalty_rate_from_year(input_file, year, "Separatist") == BAD_DATA)) {
    return BAD_DATA;
  }
  else if ((calculate_causalty_rate_from_year(input_file, year, "Republic") == FILE_READ_ERR)
           || (calculate_causalty_rate_from_year(input_file, year, "Separatist") == FILE_READ_ERR)) {
    return FILE_READ_ERR;
  }
  else {
    republic_cr = calculate_causalty_rate_from_year(input_file, year, "Republic");
    seperatist_cr = calculate_causalty_rate_from_year(input_file, year, "Separatist");
  }
  casualty_ratio = (republic_cr / seperatist_cr);
  return casualty_ratio;
}

double cost_per_hour(char *input_file, char *clone_unit){

  double sum_of_costs = 0.0;
  double total_duration_of_all_battles = 0.0;

  FILE *input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }

  typedef struct {
    char planet[MAX_NAME_LEN];
    int year;
    double duration;
    double cost;
    char outcome[5];
    char jedi[MAX_NAME_LEN];
    char clone_unit[MAX_NAME_LEN];
    int r_forces;
    int r_losses;
    int s_forces;
    int s_losses;
  } t_battle_record;

  t_battle_record br;

  while (read_battle(input_fp, &br) == 11) { //EOF isnt reached. all slots are being filled
    if (strcmp(clone_unit, br.clone_unit) == 0) {
      sum_of_costs += br.cost;
      total_duration_of_all_battles += br.duration;
    }
  }
  fclose(input_fp);
  if (total_duration_of_all_battles <= 0.0) {
    return NO_DATA;
  }
  return sum_of_costs / total_duration_of_all_battles; //cost_per_hour
}

double planetary_forces_ratio(char *input_file, char *planet){
  double number_of_battles = 0.0;
  double number_of_r_forces = 0.0;
  double number_of_s_forces = 0.0;

  FILE *input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }

  typedef struct {
    char planet[MAX_NAME_LEN];
    int year;
    double duration;
    double cost;
    char outcome[5];
    char jedi[MAX_NAME_LEN];
    char clone_unit[MAX_NAME_LEN];
    int r_forces;
    int r_losses;
    int s_forces;
    int s_losses;
  } t_battle_record;

  t_battle_record br;

  while (read_battle(input_fp, &br) == 11) {
    if ((strcmp(planet, br.planet) == 0)) {
      number_of_battles++;
      if (strcmp(br.outcome, "W")) {
        number_of_r_forces += br.r_forces;
        number_of_s_forces += br.s_forces;
      }
    }
  }
  fclose(input_fp);
  if (number_of_battles <= 0.0) {
    return NO_DATA;
  }
  return ((1 + number_of_r_forces) / ((number_of_battles) + (number_of_s_forces))); //ratio returned
}

double clone_unit_casualties_stdev(char *input_file, char *clone_unit){
  double stdev = 0.0;
  double sum_of_losses = 0.0;
  double avg_losses = 0.0;
  double number_of_battles_by_unit = 0.0;
  FILE *input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }
  typedef struct {
    char planet[MAX_NAME_LEN];
    int year;
    double duration;
    double cost;
    char outcome[5];
    char jedi[MAX_NAME_LEN];
    char clone_unit[MAX_NAME_LEN];
    int r_forces;
    int r_losses;
    int s_forces;
    int s_losses;
  } t_battle_record;
  t_battle_record br;

  while (read_battle(input_fp, &br) == 11) {
    if (strcmp(clone_unit, br.clone_unit) == 0) {
      number_of_battles_by_unit++;
      sum_of_losses += br.r_losses;
    }
  }
  avg_losses = sum_of_losses / number_of_battles_by_unit; //avg found
  rewind(input_fp);
  while (read_battle(input_fp, &br) == 11) {
    if (strcmp(clone_unit, br.clone_unit) == 0) {
     stdev += pow((br.r_losses - avg_losses),2);
    }
  }
  fclose(input_fp);
  stdev = sqrt(stdev / number_of_battles_by_unit);
  return stdev;
}

bool fallen_jedi_check(double cost, double duration, int r_forces, int r_losses){
  int integer_cost = (int)floor(cost);

  //possible error here about the def of significant digits

  bool cost_condition = false;
  if (((integer_cost % 66) == 0) || ((integer_cost / 1000) == 501)){
    cost_condition = true;
  }
  if ((cost_condition == true) && (duration > 5555.27) && (r_forces == r_losses)) {
    return true;
  }
  return false;
}

double fallen_jedi_sacrifice(char *input_file){
  double total_battles_where_jedi_fell = 0.0;
  double wins_jedi_fell = 0.0;

  FILE *input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }
  typedef struct {
    char planet[MAX_NAME_LEN];
    int year;
    double duration;
    double cost;
    char outcome[5];
    char jedi[MAX_NAME_LEN];
    char clone_unit[MAX_NAME_LEN];
    int r_forces;
    int r_losses;
    int s_forces;
    int s_losses;
  } t_battle_record;
  t_battle_record br;

  while (read_battle(input_fp, &br) == 11) {
    if (fallen_jedi_check(br.cost, br.duration, br.r_forces, br.r_losses)) {
      total_battles_where_jedi_fell++;
      if (strcmp(br.outcome, "W") == 0) {
        wins_jedi_fell++;
      }
    }
  }
  if (total_battles_where_jedi_fell == 0) {
    return NO_DATA;
  }
  return (wins_jedi_fell/ total_battles_where_jedi_fell);
}

double truncate4(double value) {
  return floor(value * 10000.0) / 10000.0;
}

int senate_report(char *input_file, char *output_file, int year) {
  FILE *input_fp = fopen(input_file, "r");
  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }
  typedef struct {
    char planet[MAX_NAME_LEN];
    int year;
    double duration;
    double cost;
    char outcome[5];
    char jedi[MAX_NAME_LEN];
    char clone_unit[MAX_NAME_LEN];
    int r_forces;
    int r_losses;
    int s_forces;
    int s_losses;
  } t_battle_record;

  t_battle_record br;
  int total_battles = 0;
  int total_battles_won = 0;
  double total_expenditure = 0.0;
  int total_republic_forces = 0;
  double total_win_cost = 0.0;

  while (read_battle(input_fp, &br) == 11) {
    if (br.year == year) {
      total_battles++;
      total_expenditure += br.cost;
      total_republic_forces += br.r_forces;
      if (strcmp(br.outcome, "W") == 0) {
        total_battles_won++;
        total_win_cost += br.cost;
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
  char *header1 = "==================================================";
  char *header2 = "== GALACTIC SENATE DEFENSE EXPENDITURE REPORT ==";
  char *header3 = "============= FOR SENATE EYES ONLY =============";
  fprintf(output_fp, "%s\n%s\n%s\n%s\n", header1, header2, header3, header1);
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
  fprintf(output_fp, "For the Republic!\n");
  fclose(output_fp);
  return total_battles_won;
}

int jedi_council_report(char *input_file, char *output_file) {
  FILE *input_fp = fopen(input_file, "r");

  if (input_fp == NULL) {
    return FILE_READ_ERR;
  }

  typedef struct {
    char planet[MAX_NAME_LEN];
    int year;
    double duration;
    double cost;
    char outcome[5];
    char jedi[MAX_NAME_LEN];
    char clone_unit[MAX_NAME_LEN];
    int r_forces;
    int r_losses;
    int s_forces;
    int s_losses;
  } t_battle_record;

  t_battle_record br;

  // Track Jedi victories
  char current_jedi[MAX_NAME_LEN] = "";
  int current_victories = 0;
  char most_victorious_jedi[MAX_NAME_LEN] = "";
  int max_victories = 0;

  // Track fallen Jedi
  typedef struct {
      char jedi[MAX_NAME_LEN];
      char clone_unit[MAX_NAME_LEN];
      char planet[MAX_NAME_LEN];
  } fallen_t;

  fallen_t fallen_heroes[1000]; // arbitrary limit
  int fallen_count = 0;

  while (read_battle(input_fp, &br) == 11) {
      // Count victories per Jedi
      if (strcmp(br.jedi, current_jedi) != 0) {
          // New Jedi group, check previous
          if (current_victories > max_victories) {
              max_victories = current_victories;
              strcpy(most_victorious_jedi, current_jedi);
          }
          strcpy(current_jedi, br.jedi);
          current_victories = 0;
      }
      if (strcmp(br.outcome, "W") == 0) current_victories++;

      // Check fallen Jedi
      if (fallen_jedi_check(br.cost, br.duration, br.r_forces, br.r_losses)) {
          strcpy(fallen_heroes[fallen_count].jedi, br.jedi);
          strcpy(fallen_heroes[fallen_count].clone_unit, br.clone_unit);
          strcpy(fallen_heroes[fallen_count].planet, br.planet);
          fallen_count++;
      }
  }
  // Check last Jedi group
  if (current_victories > max_victories) {
      max_victories = current_victories;
      strcpy(most_victorious_jedi, current_jedi);
  }

  fclose(input_fp);

  // Open output
  FILE *output_fp = fopen(output_file, "w");
  if (output_fp == NULL) {
    fclose(input_fp);
    return FILE_WRITE_ERR;
  }

  char *header1 = "===========================================";
  char *header2 = "== JEDI HIGH COUNCIL TACTICAL BRIEFING ==";
  char *header3 = "======= FOR JEDI MASTER EYES ONLY =======";

  fprintf(output_fp, "%s\n%s\n%s\n%s\n", header1, header2, header3, header1);

  fprintf(output_fp, "Most Victorious Jedi General: %s with %d victories\n",
          most_victorious_jedi, max_victories);

  fprintf(output_fp, "Jedi Casualties: %d\n", fallen_count);

  if (fallen_count > 0) {
      fprintf(output_fp, "Jedi Sacrifice Effectiveness: %.4f%%\n", fallen_jedi_sacrifice(input_file));
  } else {
      fprintf(output_fp, "Jedi Sacrifice Effectiveness: N/A\n");
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
  fprintf(output_fp, "May the Force guide our strategy to bring peace to the galaxy!\n");
  fclose(output_fp);
  return fallen_count; // number of fallen Jedi
}



