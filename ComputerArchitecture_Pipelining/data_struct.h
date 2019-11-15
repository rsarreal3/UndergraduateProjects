#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INSTRUCT 32
#define EMPTY -8 // for the max amount of stall
#define IF  0
#define ID  1
#define EX1 2
#define EX2 3
#define EX3 4
#define EX4 5
#define ME  6
#define WB  7




typedef struct instruction {
  char* opcode;
  char* label;
  char* dst_reg;
  char* src_reg1;
  char* src_reg2;
  int soonest_fwd;
  int required;
  int if_clk_cycle;
  int id_clk_cycle;
  int ex1_clk_cycle;
  int ex2_clk_cycle;
  int ex3_clk_cycle;
  int ex4_clk_cycle;
  int me_clk_cycle;
  int wb_clk_cycle;
  int has_needed_data;
  int has_needed_data2;
  int end_cycle[8];
} INSTRUCT_T;

INSTRUCT_T* CreateInstruction(char* opcode, char* label, 
			      char* dst_reg, char* src_reg1, char* src_reg2,
			      int soonest_fwd, int required, int needed_data,
			      int needed_data2);

int IsOpcode(const char* my_str);

int UpdateEndCycle(INSTRUCT_T* instruct_array[], int cycle, int curr_stage,
		   int IF_stage, int ID_stage, int EX1_stage, int EX2_stage,
		   int EX3_stage, int EX4_stage, int ME_stage, int WB_stage);

int CreateOutputFile(INSTRUCT_T* instruct_array[], int array_size);

char* IntToStr(int int_val);

#endif

