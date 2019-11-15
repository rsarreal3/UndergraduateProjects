/*datastruct.c*/
#include "data_struct.h"


INSTRUCT_T*  CreateInstruction(char* opcode, char* label,
			       char* dst_reg, char* src_reg1, char* src_reg2,
			       int soonest_fwd, int required, int needed_data,
			       int needed_data2) {
  INSTRUCT_T* node = (INSTRUCT_T*)malloc(sizeof(INSTRUCT_T));  
  node->opcode = opcode;
  node->label = label;
  // BNE has a different format
    if ((strcmp(opcode, "BNE") == 0) || (strcmp(opcode, "BEQ") == 0)) {
      node->dst_reg = src_reg2;
      node->src_reg1 = dst_reg;
      node->src_reg2 = src_reg1;
    }
    else {
    node->dst_reg = dst_reg;
    node->src_reg1 = src_reg1;
    node->src_reg2 = src_reg2;
      }
  node->soonest_fwd = soonest_fwd;
  node->required = required;
  node->has_needed_data = needed_data;
  if ((strcmp(src_reg2, "") == 0) || (strcmp(src_reg2, "N/A") == 0)) {
    node->has_needed_data2 = 1;
  }
  else {
    node->has_needed_data2 = needed_data2;
  }
  int i;
  for (i = 0 ; i < 8 ; i++) {
    node->end_cycle[i] = 0;
  }
  return node;
}

int IsOpcode(const char* my_str) {
  if ((strcmp(my_str,"ADD") == 0)    || (strcmp(my_str, "MULT") == 0) || 
      (strcmp(my_str, "SUB") == 0)   || (strcmp(my_str, "AND") == 0) ||
      (strcmp(my_str, "OR") == 0)    || (strcmp(my_str, "LW") == 0) || 
      (strcmp(my_str, "SW") == 0)    || (strcmp(my_str, "ADDI") == 0) || 
      (strcmp(my_str, "MULTI") == 0) || (strcmp(my_str,  "SUBI") == 0) || 
      (strcmp(my_str, "ANDI") == 0)  || (strcmp(my_str, "ORI") == 0) ||
      (strcmp(my_str, "LI") == 0)    || (strcmp(my_str, "LUI") == 0) || 
      (strcmp(my_str, "BEQ") == 0)   || (strcmp(my_str, "BNE") == 0) || 
      (strcmp(my_str, "J") == 0)     || (strcmp(my_str, "HLT") == 0)) {
   
    return 1;
  }
  return 0;
}


int UpdateEndCycle(INSTRUCT_T* instruct_array[], int cycle, int curr_stage,
		   int IF_stage, int ID_stage, int EX1_stage, int EX2_stage,
		   int EX3_stage, int EX4_stage, int ME_stage, int WB_stage){

  INSTRUCT_T* this_instruct = instruct_array[curr_stage];
  if (curr_stage == IF_stage) {
    this_instruct->end_cycle[IF] = cycle;
  }
  else if (curr_stage == ID_stage) {
    this_instruct->end_cycle[ID] = cycle;
  }
  else if (curr_stage == EX1_stage) {
    this_instruct->end_cycle[EX1] = cycle;
  }
  else if (curr_stage == EX2_stage) {
    this_instruct->end_cycle[EX2] = cycle;
  }
  else if (curr_stage == EX3_stage) {
    this_instruct->end_cycle[EX3] = cycle;
  }
  else if (curr_stage == EX4_stage) {
    this_instruct->end_cycle[EX4] = cycle;
  }
  else if (curr_stage == ME_stage) {
    this_instruct->end_cycle[ME] = cycle;
  }
  else if (curr_stage == WB_stage) { // WB_stage
    this_instruct->end_cycle[WB] = cycle;
  }
  else {
    return 0;
  }
  return 1;
}

int CreateOutputFile(INSTRUCT_T* instruct_array[], int array_size) {
  char c;
  FILE* my_file;
  my_file = fopen("output.txt", "w");
  if (my_file == NULL) {
    return -1;
  }


  //  int array_size = sizeof(instruct_array)/sizeof(instruct_array[0]);
  printf("size of array: %d\n", array_size);
  int i, j;
  INSTRUCT_T* my_instruct;
  int str_i;

  fputs("Cycle Number for Each Stage \t\tIF\tID\tEX4\tMEM\tWB\n", my_file);

  for(i = 0 ; i < array_size ; i++) {
    printf("FORi\n");
    my_instruct = instruct_array[i];
    str_i = 0;
    c = (my_instruct->label)[str_i];

    while (c != '\0') {
      c = (my_instruct->label)[str_i];
      fputc(c, my_file);
      str_i++;      
    }
    
    fputc('\t', my_file);
    str_i = 0;
    c = (my_instruct->opcode)[str_i];
    while(c != '\0') {
      c = (my_instruct->opcode)[str_i];
      fputc(c, my_file);
      str_i++;
    }
    //fputc('\0', my_file);
    fputs("   ", my_file);
    if ((strcmp(my_instruct->opcode, "BNE") == 0) ||
	(strcmp(my_instruct->opcode, "BEQ") == 0)) {
      str_i = 0;
      c = (my_instruct->src_reg1)[str_i];
      while(c != '\0') {
	c = (my_instruct->src_reg1)[str_i];
	fputc(c, my_file);
	str_i++;
      }
      fputs(",", my_file);
      str_i = 0;
      c = (my_instruct->src_reg2)[str_i];
      while(c != '\0') {
	c = (my_instruct->src_reg2)[str_i];
	fputc(c, my_file);
	str_i++;
      }
      fputs(",", my_file);
      str_i = 0;
      c = (my_instruct->dst_reg)[str_i];
      while(c != '\0') {
	c = (my_instruct->dst_reg)[str_i];
	fputc(c, my_file);
	str_i++;
      }
    }
    else {
      str_i = 0;
      c = (my_instruct->dst_reg)[str_i];
      while(c != '\0') {
	c = (my_instruct->dst_reg)[str_i];
	fputc(c, my_file);
	str_i++;
      }

      fputs(",", my_file);
      str_i = 0;
      c = (my_instruct->src_reg1)[str_i];

      while(c != '\0') {
	c = (my_instruct->src_reg1)[str_i];
	fputc(c, my_file);
	str_i++;
      }
      fputs(",", my_file);
      str_i = 0;
      c = (my_instruct->src_reg2)[str_i];
      while(c != '\0') {
	c = (my_instruct->src_reg2)[str_i];
	fputc(c, my_file);
	str_i++;
      }
    }
    fputs("\t\t", my_file);
    for (j = 0 ; j < 8 ; j++) {
      printf("FOR ty\n");
      char* num_str = IntToStr((my_instruct->end_cycle)[j]);
      str_i = 0;
      printf("num_str = %s\n", num_str);
      c = num_str[str_i];
      while(c != '\0') {
	c = num_str[str_i];
	fputc(c, my_file);
	str_i++;
      }
      fputc('\t', my_file);
    }
    fputs("\n", my_file);
  }
  
  //  char* num_str1, num_str2, num_str3, num_str4;
  fputs("Total number of access requests for instruction cache: ", my_file);
  //  num_str1 =
  fputs("\n", my_file);
  fputs("Number of instruction cache hits: ", my_file);
  // something
  fputs("\n", my_file);
  fputs("Total number of access requests for data cache: ", my_file);
  // something else
  fputs("\n", my_file);
  fputs("Number of data cache hits: ", my_file);
  // something else again
  fputs("\n", my_file);
  fclose(my_file);
  return 0;  

  // note to future self: instead of using tabs, do a while
  // the line is not a certain length, tack on some spaces
  // to make more attractive
}




char* IntToStr(int int_val) { // will only handle up to hundreds place
  char c; //= (char*)malloc(sizeof(char));
  char* int_str = (char*)malloc(4 * sizeof(char));
  
  if (int_val < 0) {
    printf("hi1\n");
    strcpy(int_str, "-");
    printf("hi2\n");
    int_val = int_val * -1;
  }
  if (int_val <= 10) {
    //c = int_val + 48; // the difference between int 0 and char '0'
    c = 3 + 48;
    printf("hi3\n");
    strcat(int_str, &c);
    printf("hi4\n");
  }
  else if (int_val <= 100) {
    /*    c = (int_val / 10) + 48;
     printf("hi5\n");
    strcat(int_str, &c);
    printf("hi6\n"); */
    //    c = (int_val % 10) + 48;
    c = 3 + 48;
    strcat(int_str, &c);
  }
  else if (int_val < 1000) {
    /* c = (int_val / 100) + 48;
    strcat(int_str, &c);
    c = ((int_val - (int_val / 100)) / 10) + 48;
    strcat(int_str, &c);*/
    //c = (int_val % 10) + 48;
    c = 3 + 48;
    strcat(int_str, &c);
  } 
  return int_str;
}
