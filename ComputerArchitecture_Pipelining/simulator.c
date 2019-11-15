/* File: simulator.c
 * Author: Ressa Reneth Sarreal
 * Date: 11/14/2017
 * E-mail: saressa1@umbc.edu
 * Description:
 */ 

#include "data_struct.h"
#include "pipelining.h"
#include "memory.h"

static int IF_stage = -1;
static int ID_stage = -2;
static int EX1_stage = -3;
static int EX2_stage = -4;
static int EX3_stage = -5;
static int EX4_stage = -6;
static int ME_stage = -7;
static int WB_stage = -8;

int main(int argc, char* argv[]) {
  
  INSTRUCT_T* instruct_array[MAX_INSTRUCT];
  int array_size = 0;
  int reg_array[32];
  long mem_array[1280];
  //int 
  if (argc != 2){    
    printf("Invalid input. Please try again.\n");
    return -1;
  }
  
  FILE * inputFile;
  char* filename = argv[1];
  
  inputFile = fopen(filename, "r");

  if (inputFile == NULL) { // couldn't be opened
    printf("Invalid Filename. Please try again.\n");
    return -2;
  }

  // Read in & parse input file for instructions. Ignore comments.

  
  char line[256];
  char* save;
  char* token;
  char s[5] = ", \t\n:";
  int count, index;
  int hasLabel = 0;
  int i;
  index = 0;
  
  while (fgets(line, sizeof(line), inputFile)) { // while not eof 
    char * label, * opcode, * dst, * src1, * src2;
    int soon_fwd, req;
    
    label = (char*) malloc(32 * sizeof(char));
    opcode = (char*) malloc(32 * sizeof(char));
    dst = (char*) malloc(32 * sizeof(char));
    src1 = (char*) malloc(32 * sizeof(char));
    src2 = (char*) malloc(32 * sizeof(char));

    save = line;
    count = 0;
    hasLabel = 0;
    while((token = (strtok_r(save, s, &save)))) {

      // capitalize all characters in token
      i = 0;
      while(token[i]) {
	// if token is lowercase letter
	if (token[i] >= 97 && token[i] <= 122) {
	  token[i] = toupper(token[i]);
	}
	i++;
      }
      
      //handle comments:
      if (token[0] == '#') {
	//no more code afterwards so break
	break;
      }
      
      // for first arg, check if token = opcode list of strings
      // if not, then it is a label. store as a label. 
      // if it is, store as an opcode.
      else if (count == 0) {
	if (IsOpcode(token)) {
	  strcpy(opcode,token);
	  array_size++;
	}
	else {
	  strcpy(label,token);
	  hasLabel = 1;
	}
	count++;
      }
      
      else {
	
	if (hasLabel) {
	  if (count == 1) {
	    if (strlen(token) != 0) {
	      strcpy(opcode, token);
	      array_size++;
	    }
	    //	    else {
	    //  strcpy(opcode, "N/A");
	    //}
	  }
	  else if (count == 2) {
	    if (strlen(token) != 0) {
	      strcpy(dst, token);
	    }
	    //	    else {
	    //  strcpy(dst, "N/A");
	    //}
	  }
	  else if (count == 3) {
	    if (strlen(token) != 0) {
	      strcpy(src1, token);
	    }
	    // else {
	    //  strcpy(src1, "N/A");
	    //}
	  }
	  else if (count == 4) {
	    if (strlen(token) != 0) {
	      strcpy(src2, token);
	    }
	    // else {
	    //  strcpy(src2, "N/A");
	    //}
	  }
	}
	
	else { // no label
	  // strcpy(label,"N/A");
	  if (count == 1) {
	    if (strlen(token) != 0) {
	      strcpy(dst, token);
	    }
	    //else {
	    //  strcpy(dst, "N/A");
	    //}
	  }
	  else if (count == 2) {
	    if (strlen(token) != 0) {
	      strcpy(src1, token);
	    }
	    //else {
	    //  strcpy(src1, "N/A");
	    //}
	  }
	  else if (count == 3) {
	    if (strlen(token) != 0) {
	      strcpy(src2, token);
	    }
	    //else {
	    //  strcpy(src2, "N/A");
	    //}
	  }
	}
	count++;
      }
    }
    
    soon_fwd = GetSoonestFwd(opcode);
    req = GetRequiredStage(opcode);
     
    INSTRUCT_T* new_instruction = CreateInstruction(opcode, label, dst, src1, src2, 
						    soon_fwd, req, 0, 0);
    instruct_array[index] = new_instruction;
    index++;
    
    printf("INSTURCTION CREATED: \n");
    printf("Label: %s\n", new_instruction->label);
    printf("Opcode: %s\n", new_instruction->opcode);
    printf("Destination Reg: %s\n", new_instruction->dst_reg);
    printf("Source1 Reg: %s\n", new_instruction->src_reg1);
    printf("Source2 Reg: %s\n", new_instruction->src_reg2);
    printf("Soonest Fwd: %d\n", new_instruction->soonest_fwd);
    printf("Required stage: %d\n\n", new_instruction->required);
    
  }
  /*
  printf("Instructions stored:\n");
  for (i = 0 ; i < index ; i++ ){
    printf("%d %s\n", i, instruct_array[i]->opcode);
  }
  printf("\n");
  */

  // Now handle pipelining:
  int cycle = 0;
  int executing = 1;
  int stall_flags = 0b00000000;
  int num_instruct = index;


  printf ("number of instructions : %d\n", num_instruct);  
  while (executing) {
    stall_flags = 0b00000000;
    
    // if the final instruction just completed, then stop executing.
    
    printf("--------------------clk cycle %d----------------------\n", cycle);
    printf("IF_stage  = %d\n", IF_stage);
    printf("ID_stage  = %d\n", ID_stage);
    printf("EX1_stage = %d\n", EX1_stage);
    printf("EX2_stage = %d\n", EX2_stage);
    printf("EX3_stage = %d\n", EX3_stage);
    printf("EX4_stage = %d\n", EX4_stage);
    printf("ME_stage  = %d\n", ME_stage);
    printf("WB_stage  = %d\n", WB_stage);
    
    if (WB_stage == num_instruct - 1) {
      executing = 0;
      break;
    }
    
    // initialize
    if (cycle == 0) {
      if (IF_stage < 0) { // if it is not initialized yet
	// check the cache & do an access
	// for now we will just do an access
	IF_stage = 0; // initialize to the first instruction;
	ID_stage = IF_stage - 1;
	EX1_stage = ID_stage - 1;
	EX2_stage = EX1_stage - 1;
	EX3_stage = EX2_stage - 1;
	EX4_stage = EX3_stage - 1;
	ME_stage = EX4_stage - 1;
	WB_stage = ME_stage - 1;
      }
    }
    
    
    else {
      // check if WB depends on somehting occurring:
      if ((WB_stage >= 0) &&
	  IsDependent(instruct_array, WB_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, EX3_stage,
		      EX4_stage, ME_stage, WB_stage)) {
	if (!CanFwd(instruct_array, WB_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, EX3_stage,
		    EX4_stage, ME_stage, WB_stage)) {
	  stall_flags |= (1 << WB);
	  // do not update the IF instruction
	  // everything that has an instruction index greater than this one, should also stall
	}
      }
      
      // check ME
      else if ((ME_stage >= 0) &&
	       IsDependent(instruct_array, ME_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
			   EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	if (!CanFwd(instruct_array, ME_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
		    EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	  stall_flags |= (1 << ME);
	}
      }

      // check EX4
      else if ((EX4_stage >= 0) &&
	       IsDependent(instruct_array, EX4_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
			   EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	if (!CanFwd(instruct_array, EX4_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
		    EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	  stall_flags |= (1 << EX4);
	}
      }
      
      // check EX3
      else if ((EX3_stage >= 0) &&
	       IsDependent(instruct_array, EX3_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
			   EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	if (!CanFwd(instruct_array, EX3_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
		    EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	  stall_flags |= (1 << EX3);
	}
      }
      
      // check EX2
      else if ((EX2_stage >= 0) &&
	       IsDependent(instruct_array, EX2_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
			   EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	if (!CanFwd(instruct_array, EX2_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
		    EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	  stall_flags |= (1 << EX2);
	}
      }
      
      // check EX1
      else if ((EX1_stage >= 0) &&
	       IsDependent(instruct_array, EX1_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
			   EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	if (!CanFwd(instruct_array, EX1_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
		    EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	  stall_flags |= (1 << EX1);
	}
      }
      
      // check ID
      else if (ID_stage >= 0) {
	// update the materials obtained @ ID stage because that's when
	// it naturally does that, even if it doesnt need to
	ForwardAvailableResources(instruct_array, ID_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
				  EX3_stage, EX4_stage, ME_stage, WB_stage);
      
	if (IsDependent(instruct_array, ID_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
			EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	  if (!CanFwd(instruct_array, ID_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
		      EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	    stall_flags |= (1 << ID);
	  }
	}
      }
      
      // check IF
      else if ((IF_stage >= 0) &&
	       IsDependent(instruct_array, IF_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
			   EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	if (!CanFwd(instruct_array, IF_stage, IF_stage, ID_stage, EX1_stage, EX2_stage, 
		    EX3_stage, EX4_stage, ME_stage, WB_stage)) {
	  stall_flags |= (1 << IF);
	}
      }
      
      // if you are not stalled, or are greater than the stalled value, then you can proceed,
      // others should remain the same

      printf("stall flags: %d\n", stall_flags);

      
      if (stall_flags >= (1 << WB)) {
	//NOP
	printf("ERROR: THIS SHOULD NEVER HAPPEN!!!");
      }
      else if (stall_flags >= (1 << ME)) {
	//check for next instruction
	WB_stage = EMPTY;
      }
      else if (stall_flags >= (1 << EX4)) {
	WB_stage = ME_stage;
	ME_stage = EMPTY;
      }	
      else if (stall_flags >= (1 << EX3)) {
	WB_stage = ME_stage;
	ME_stage = EX4_stage;
	EX4_stage = EMPTY;


      }
      else if (stall_flags >= (1 << EX2)) {
	WB_stage = ME_stage;
	ME_stage = EX4_stage;
	EX4_stage = EX3_stage;
	EX3_stage = EMPTY;
      }
      else if (stall_flags >= (1 << EX1)) {
	WB_stage = ME_stage;
	ME_stage = EX4_stage;
	EX4_stage = EX3_stage;
	EX3_stage = EX2_stage;
	EX2_stage = EMPTY;
      }
      else if (stall_flags >= (1 << ID)) {
	WB_stage = ME_stage;
	ME_stage = EX4_stage;
	EX4_stage = EX3_stage;
	EX3_stage = EX2_stage;
	EX2_stage = EX1_stage;
	EX1_stage = EMPTY;
      }
      else if (stall_flags >= (1 << IF)) {
	WB_stage = ME_stage;
	ME_stage = EX4_stage;
	EX4_stage = EX3_stage;
	EX3_stage = EX2_stage;
	EX2_stage = EX1_stage;
	EX1_stage = ID_stage;
	ID_stage = EMPTY;
      }
      else {
	WB_stage = ME_stage;
	ME_stage = EX4_stage;
	EX4_stage = EX3_stage;
	EX3_stage = EX2_stage;
	EX2_stage = EX1_stage;
	EX1_stage = ID_stage;
	ID_stage = IF_stage;
	IF_stage++;
      }

     






      // checking for if values are too high. set them as invalid
      if (WB_stage > (num_instruct - 1)) {
	WB_stage = (num_instruct * - 1);
      }
      if (ME_stage > (num_instruct - 1)) {
	ME_stage = (num_instruct * - 1);
      }
      if (EX4_stage > (num_instruct - 1)) {
	EX4_stage = (num_instruct * - 1);
      }
      if (EX3_stage > (num_instruct - 1)) {
	EX3_stage = (num_instruct * - 1);
      }
      if (EX2_stage > (num_instruct - 1)) {
	EX2_stage = (num_instruct * - 1);
      }
      if (EX1_stage > (num_instruct - 1)) {
	EX1_stage = (num_instruct * - 1);
      }
      if (ID_stage > (num_instruct - 1)) {
	ID_stage = (num_instruct * - 1);
      }
      if (IF_stage > (num_instruct - 1)) {
	IF_stage = (num_instruct * - 1);
      }     
    }

    printf("--------------------clk cycle %d end----------------------\n", cycle);
    printf("IF_stage  = %d\n", IF_stage);
    printf("ID_stage  = %d\n", ID_stage);
    printf("EX1_stage = %d\n", EX1_stage);
    printf("EX2_stage = %d\n", EX2_stage);
    printf("EX3_stage = %d\n", EX3_stage);
    printf("EX4_stage = %d\n", EX4_stage);
    printf("ME_stage  = %d\n", ME_stage);
    printf("WB_stage  = %d\n", WB_stage);
    printf("\n\n");
    
    cycle++;
  }


  /*
  int make_output;
  if (!(make_output = CreateOutputFile(instruct_array, array_size ))){
    fprintf(stderr, "ERROR: Could not create output file.\n");
    return -4;
  }
  */

  int check = InitDataMemory(&mem_array[0x100], "data.txt");
  printf("check: %d\n", check);
  check = RegStrToInt("R32");
  
  return 0;
}





      /*
      if (stall_flags >= (1 << WB)) {
	//NOP
	printf("ERROR: THIS SHOULD NEVER HAPPEN!!!");
      }
      else if (stall_flags >= (1 << ME)) {
	WB_stage = EMPTY;
      }
      else if (stall_flags >= (1 << EX4)) {
	ME_stage = EMPTY;
	WB_stage++;
      }	
      else if (stall_flags >= (1 << EX3)) {
	EX4_stage = EMPTY;
	ME_stage++;
	WB_stage++;
      }
      else if (stall_flags >= (1 << EX2)) {
	EX3_stage = EMPTY;;
	EX4_stage++;
	ME_stage++;
	WB_stage++;
      }
      else if (stall_flags >= (1 << EX1)) {
	EX2_stage = EMPTY;
	EX3_stage++;
	EX4_stage++;
	ME_stage++;
	WB_stage++;
      }
      else if (stall_flags >= (1 << ID)) {
	EX1_stage = EMPTY;
	EX2_stage++;
	EX3_stage++;
	EX4_stage++;
	ME_stage++;
	WB_stage++;
      }
      else if (stall_flags >= (1 << IF)) {
	ID_stage++;
	EX1_stage++;
	EX2_stage++;
	EX3_stage++;
	EX4_stage++;
	ME_stage++;
	WB_stage++;
      }
      else {
	IF_stage++;
	ID_stage++;
	EX1_stage++;
	EX2_stage++;
	EX3_stage++;
	EX4_stage++;
	ME_stage++;
	WB_stage++;
      }

      */


