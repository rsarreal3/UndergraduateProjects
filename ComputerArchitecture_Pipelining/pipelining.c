/* pipelining.c*/

#include "data_struct.h"
#include "pipelining.h"



int GetSoonestFwd(const char* opcode) {

  // Return the soonest stage 
  if ((strcmp(opcode, "J") == 0) || (strcmp(opcode, "BEQ") == 0) ||
	   (strcmp(opcode, "BNE") == 0) || (strcmp(opcode, "LI") == 0) ||
	   (strcmp(opcode, "LUI") == 0)) {
    return EX1;
  }
 
  else if ((strcmp(opcode, "AND") == 0) || (strcmp(opcode, "ANDI") == 0) ||
	   (strcmp(opcode, "OR") == 0) || (strcmp(opcode, "ORI") == 0)) {
    
    return EX2;
  }
  
  else if ((strcmp(opcode, "ADD") == 0) || (strcmp(opcode, "SUB") == 0) ||
	  (strcmp(opcode, "ADDI") == 0) || (strcmp(opcode, "SUBI") == 0)) {
    
    return EX3;
  }

  else if ((strcmp(opcode, "MULT") == 0) || (strcmp(opcode, "MULTI") == 0)) {
    return ME;
  }
  
  else if (strcmp(opcode, "LW") == 0) {
    return WB;
  }

  // stores do not affect the data
  else if (strcmp(opcode, "SW") == 0) {
    return IF;
  }
  
  else if (strcmp(opcode, "HLT") == 0) {
    return IF; // kind of like it doesnt really matter
  }
  
  else {
    return -1; // error
  }
}


int GetRequiredStage(const char* opcode) {
  if ((strcmp(opcode, "LUI") == 0 ) || (strcmp(opcode, "LI") == 0) ||
      (strcmp(opcode, "J") == 0) || (strcmp(opcode, "BEQ") == 0) ||
      (strcmp(opcode, "BNE") == 0)) {
    
    return ID;
  }
  
  else if ((strcmp(opcode, "AND") == 0) || (strcmp(opcode, "ANDI") == 0) ||
	   (strcmp(opcode, "OR") == 0) || (strcmp(opcode, "ORI") == 0) ||
	   (strcmp(opcode, "ADD") == 0) ||(strcmp(opcode, "SUB") == 0) ||
	   (strcmp(opcode, "ADDI") == 0) || (strcmp(opcode, "SUBI") == 0) ||
	   (strcmp(opcode, "MULT") == 0) || (strcmp(opcode, "MULTI") == 0)) {
    return EX1; 
  }
  
  else if (strcmp(opcode, "SW") == 0) {
    return ME;
  }
  
  else if (strcmp(opcode, "LW") == 0) {
    return WB;
  }

  else if (strcmp(opcode, "HLT") == 0) {
    return WB; // kind of like a never
  }

  else {
    return -1;
  }
}
  


int IsDependent(INSTRUCT_T * instruct_array[], int curr_stage, int IF_stage, int ID_stage,
		int EX1_stage, int EX2_stage, int EX3_stage, int EX4_stage, int ME_stage,
		int WB_stage) {

  // edge case (if this is the first IF)
  if (curr_stage == 0) {
    // no dependencies.
    return 0;
  }
  
  // form the possibly dependent by checking to see if any instructions come before
  else {
    if (GetDependencies(instruct_array, curr_stage, IF_stage, ID_stage, EX1_stage,
			EX2_stage, EX3_stage, EX4_stage, ME_stage, WB_stage) > 0) {
      return 1;   
    }
    else {
      return 0;
    }
  }
} 




int CanFwd(INSTRUCT_T * instruct_array[], int curr_stage, int IF_stage, int ID_stage,
	   int EX1_stage, int EX2_stage, int EX3_stage, int EX4_stage, int ME_stage,
	   int WB_stage) {

  printf("CURRENT INSTRUCTION CAN FWD FUNCT = %s\n", instruct_array[curr_stage]->opcode);
  int dependencies = GetDependencies(instruct_array, curr_stage, IF_stage, ID_stage, 
				     EX1_stage, EX2_stage, EX3_stage, EX4_stage, 
				     ME_stage, WB_stage);
  printf("dependencies found: %d\n", dependencies);
  // the masks represent where forwarding is available (@0s)
  // wait I have to represent the instruction here too
  // if MULT, i cant fwd unless it is in EX4

  int mask = 0b11111111;
  // check if the data can even be forwarded, 
  // mix mask and the ID\EX1\ME mask with this one
  
  // if the instruction has all that it needs then just keep going.
  if ((instruct_array[curr_stage]->has_needed_data) && 
      ( instruct_array[curr_stage]->has_needed_data2)) {
    return 1;
  }
  
  int this_stage;
  if (curr_stage == IF_stage) {
    this_stage = IF;
  }
  
  else if (curr_stage == ID_stage) {
    this_stage = ID;
  }
  
  else if (curr_stage == EX1_stage) {
    this_stage = EX1;
  }
  
  else if (curr_stage == EX2_stage) {
    this_stage = EX2;
  }
  else if (curr_stage == EX3_stage) {
    this_stage = EX3;
  }
  else if (curr_stage == EX4_stage) {
    this_stage = EX4;
  }
  else if (curr_stage == ME_stage) { 
    this_stage = ME;
  }  
  else { // WB
    this_stage = WB;
  }

  // if the items of interest are not necessary at this stage, just keep going
  if (instruct_array[curr_stage]->required != this_stage) {
    return 1;
  }
  
  // if there is a dependency on the IF stage
  if (dependencies & (1 << IF)) {
    printf("IF\n");
    // if it can forward at the stage it is presently, then adjust the mask as so
    // if soonest I can fwd is before or at the  current stage
    if (instruct_array[IF_stage]->soonest_fwd <= IF) {
      printf("IF masking\n");
      mask &= (0 << IF);
    }
  }
  
  if (dependencies & (1 << ID)) {
    printf("ID\n");
    if (instruct_array[ID_stage]->soonest_fwd <= ID) {
      printf("ID masking\n");
      mask &= (0 << ID);
    }
  }

  if (dependencies & (1 << EX1)) {
    printf("EX1\n");
    if (instruct_array[EX1_stage]->soonest_fwd <= EX1) {
      printf("EX1 masking\n");     
	mask &= (0 << EX1);
    }
  }

  if (dependencies & (1 << EX2)) {
    printf("EX2\n");
    if (instruct_array[EX2_stage]->soonest_fwd <= EX2) {
      printf("EX2 masking\n");
      mask &= (0 << EX2);
    }
  }

  if (dependencies & (1 << EX3)) {
    printf("EX3\n");
    if (instruct_array[EX3_stage]->soonest_fwd <= EX3) {
      printf("EX3 masking\n");
      mask &= (0 << EX3);
    }
  }

  if (dependencies & (1 << EX4)) {
    printf("EX4\n");
    if (instruct_array[EX4_stage]->soonest_fwd <= EX4) {
      printf("EX4 masking\n");
      mask &= (0 << EX4);
    }
  }

  if (dependencies & (1 << ME)) {
    printf("ME\n");
    if (instruct_array[ME_stage]->soonest_fwd <= ME) {
      printf("ME masking\n");
      mask &= (0 << ME);
    }
  }
  
if (dependencies & (1 << WB)) {
    printf("WB\n");
    if (instruct_array[WB_stage]->soonest_fwd <= WB) {
      printf("WB masking\n");
      mask &= (0 << WB);
    }
  }

// I can only forward at these stages. 
// Where there is a 0, that is the stage that can forward
// to the current stage instruction
  int ID_mask =  0b10100011;
  int EX1_mask = 0b10100111;
  int ME_mask =  0b10111111;

 printf("mask: %d\n",mask);

// shift the masks over since i say that the forwarding occurs after the stage?
/*
 int ID_mask =  0b01000111;
 int EX1_mask = 0b01001111;
 int ME_mask =  0b01111111;
*/
  // need to check for the most recent instruction dependency.
  // 

  int full_mask;


  // Forwarding is only available at certain stages
  if (this_stage == ID) {
    full_mask = (ID_mask | mask) & dependencies;
    // full mask tells me what can fwd at this time
  }
  else if (this_stage == EX1) {
    full_mask = (EX1_mask | mask) & dependencies;
  }
  else if (this_stage == ME) {
    full_mask = (ME_mask | mask) & dependencies;
  }
  else {
    full_mask = 0b11111111 & dependencies;
  }
  printf("BEFORE SET HAS NEEDED\n");
  SetHasNeeded(instruct_array, full_mask, curr_stage, IF_stage, ID_stage, 
	       EX1_stage, EX2_stage, EX3_stage, EX4_stage, 
	       ME_stage, WB_stage);
  printf("FULL MASK : %d\n", full_mask);
  printf("has src 1 = %d\n", instruct_array[curr_stage]->has_needed_data);
  printf("has src 2 = %d\n", instruct_array[curr_stage]->has_needed_data2);

  
  dependencies &= full_mask;    

  /*
  if ((curr_stage == IF_stage) &&
	instruct_array[curr_stage]->required == IF) {
      // check which register is satisfied
      if (instruct_array[curr_stage]
      instruct_array[curr_stage]->has_needed_data = 1;
    }

    else if ((curr_stage == ID_stage) &&
	     instruct_array[curr_stage]->required == ID) {
      instruct_array[curr_stage]->has_needed_data = 1;
    }

    else if ((curr_stage == EX1_stage) &&
	     instruct_array[curr_stage]->required == EX1) {
      instruct_array[curr_stage]->has_needed_data = 1;
    }

    else if ((curr_stage == EX2_stage) &&
	     instruct_array[curr_stage]->required == EX2) {
      instruct_array[curr_stage]->has_needed_data = 1;
    }
    
    else if ((curr_stage == EX3_stage) &&
	     instruct_array[curr_stage]->required == EX3) {
      instruct_array[curr_stage]->has_needed_data = 1;
    }
    
    else if ((curr_stage == EX4_stage) &&
	     instruct_array[curr_stage]->required == EX4) {
      instruct_array[curr_stage]->has_needed_data = 1;
    }
    
    else if ((curr_stage == ME_stage) &&
	     instruct_array[curr_stage]->required == ME) {
      instruct_array[curr_stage]->has_needed_data = 1;
    }
    
    else if ((curr_stage == WB_stage) &&
	     instruct_array[curr_stage]->required == WB) {
      instruct_array[curr_stage]->has_needed_data = 1;
    }
  */

  // if we forwarding is possible or if there were no dependencies, you can fwd
  if (!dependencies) {
    printf("dependencies from can fwd: %d\n", dependencies);
    printf("CanFwd = TRUE\n");
    return 1;
  }
  
  // if dependencies still remain even after forwarding, you cannot fwd
  else {
    printf("CanFwd = FALSE\n");
    return 0;
  } 
}





int GetDependencies(INSTRUCT_T * instruct_array[], int curr_stage, int IF_stage, 
		      int ID_stage, int EX1_stage, int EX2_stage, int EX3_stage, 
		     int EX4_stage, int ME_stage, int WB_stage) {

    
  //   printf ("\n----GETTING DEPENDENCIES------\n");
  int i = 0;
  int temp_array[8];
  int my_stage;
  //LSB = IF ; MSB = WB
  int dependencies = 0b00000000;
  printf("opcode = %s\ncurrent instruction: %d\n", instruct_array[curr_stage]->opcode, curr_stage);
  printf ("instruction needs something @ %d\n", instruct_array[curr_stage]->required);
  printf("instruction can fwd @ %d\n\n", instruct_array[curr_stage]->soonest_fwd);
  

  //does the instruction even really rely on another instruction right now?
  if (curr_stage == IF_stage) {
    my_stage = IF;
    if (instruct_array[curr_stage]->required != IF) {
      return 0b00000000;
    }
  }
  
  else if (curr_stage == ID_stage) {
    my_stage = ID;  
    if (instruct_array[curr_stage]->required != ID) {
      return 0b00000000;
    }
  }

  else if (curr_stage == EX1_stage) {
    my_stage = EX1;
    if (instruct_array[curr_stage]->required != EX1) {
      return 0b00000000;
    }
  }

  else if (curr_stage == EX2_stage) {
    my_stage = EX2; 
    if (instruct_array[curr_stage]->required != EX2) {
      return 0b00000000;
    }
  }

  else if (curr_stage == EX3_stage) {
    my_stage = EX3;
    if (instruct_array[curr_stage]->required != EX3) {
      return 0b00000000;
    }
  }

  else if (curr_stage == EX4_stage) { 
    my_stage = EX4; 
    if (instruct_array[curr_stage]->required != EX4) {
      return 0b00000000;
    }
  }

  else if (curr_stage == ME_stage) {
    my_stage = ME; 
    if (instruct_array[curr_stage]->required != ME) {
      return 0b00000000;
    }
  }

  else if (curr_stage == WB_stage) {
    my_stage = WB; 
    if (instruct_array[curr_stage]->required != WB) {
      return 0b00000000;
    }
  }
  
  // if the required stage is the one we are at now
  if (instruct_array[curr_stage]->required == my_stage) {
    // form the possibly dependent by checking to see if any instructions come before
    if (curr_stage > IF_stage) {
      temp_array[0] = IF_stage;
    }
    else {
      temp_array[0] = -1;
    }
    
    if (curr_stage > ID_stage) {
      temp_array[1] = ID_stage;
    }
    else {
      temp_array[1] = -1;
    }
    
    if (curr_stage > EX1_stage) {
      temp_array[2] = EX1_stage;
    }
    else {
      temp_array[2] = -1;
    }
    
    if (curr_stage > EX2_stage) {
      temp_array[3] = EX2_stage;
    }
    else {
      temp_array[3] = -1;
    }
    
    if (curr_stage > EX3_stage) {
      temp_array[4] = EX3_stage;
    }
    else {
      temp_array[4] = -1;
    }
    
    if (curr_stage > EX4_stage) {
      temp_array[5] = EX4_stage;
    }
    else {
      temp_array[5] = -1;
    }
    
    if (curr_stage > ME_stage) {
      temp_array[6] = ME_stage;
    }
    else {
      temp_array[6] = -1;
    }
    
    if (curr_stage > WB_stage) {
      temp_array[7] = WB_stage;
    }
    else {
      temp_array[7] = -1;
    }
  
    // TESTING
    /*    
    printf("Contents of temp_array\n");
    for (i = 0 ; i < 8 ; i++ ) {
      printf("temp_array[%d] = %d\n", i, temp_array[i]);
    }  
    printf("\n");

    printf("MY_STAGE = %d\n", my_stage);
    
    */

      // check if any of those
    for (i = 0 ; i < 8 ; i++) {
      // check the indexes that may be dependent if their registers affect the curr instruct
      if ((temp_array[i] >=0) && (curr_stage >= 0)) {
	if ((strcmp(instruct_array[curr_stage]->src_reg1, 
		    instruct_array[temp_array[i]]->dst_reg) == 0)) {
	  // if the item already has the item, so if src 1 is already stored, not dependent
	  if (!instruct_array[curr_stage]->has_needed_data) {
	    dependencies |= (1 << i); 
	  }
	}
	if ((strcmp(instruct_array[curr_stage]->src_reg2,
		    instruct_array[temp_array[i]]->dst_reg) == 0)) {
	  if (!instruct_array[curr_stage]->has_needed_data2) {
	    dependencies |= (1 << i); 
	  }  
	}
      }
    }
  }
  //  printf("dependencies = %d\n", dependencies);
  return dependencies;
}



 void SetHasNeeded(INSTRUCT_T * instruct_array[], int mask, int curr_stage, int IF_stage, 
		      int ID_stage, int EX1_stage, int EX2_stage, int EX3_stage, 
		     int EX4_stage, int ME_stage, int WB_stage) {
   if ((curr_stage != IF_stage) && (IF_stage >= 0)) {
     if (!(mask & (1 << IF))) {
       // IF can fwd at this time, and the current instruct depended on it
       if (strcmp(instruct_array[curr_stage]->src_reg1, instruct_array[IF_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data = 1;
       }
       if (strcmp(instruct_array[curr_stage]->src_reg2, instruct_array[IF_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data2 = 1;
       }
     }
   }
   if ((curr_stage != ID_stage) && (ID_stage >= 0)) {   
     printf("SETTING THAT THE INSTRUCT HAS WHAT IT NEEDS IN ID STAGE\n");
     if (!(mask & (1 << ID))) {
       printf("If ID can fwd to here\n");
       // IF can fwd at this time, and the current instruct depended on it
       if (strcmp(instruct_array[curr_stage]->src_reg1, instruct_array[ID_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data = 1;
       }
       if (strcmp(instruct_array[curr_stage]->src_reg2, instruct_array[ID_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data2 = 1;
       }
     }
   }
   
   if ((curr_stage != EX1_stage) && (EX1_stage >= 0)) {
     if (!(mask & (1 << EX1))) {
       // IF can fwd at this time, and the current instruct depended on it
       if (strcmp(instruct_array[curr_stage]->src_reg1, instruct_array[EX1_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data = 1;
       }
       if (strcmp(instruct_array[curr_stage]->src_reg2, instruct_array[EX1_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data2 = 1;
       }
     }
   }
   if ((curr_stage != EX2_stage) && (EX2_stage >= 0)) {
     printf("SETTING THAT THE INSTRUCT HAS WHAT IT NEEDS IN EX2 STAGE\n");
     printf("mask & (1 << EX2))) = %d\n", (mask&(1<<EX2)));
     if (!(mask & (1 << EX2))) {
       printf("1\n");
       // IF can fwd at this time, and the current instruct depended on it
       if (strcmp(instruct_array[curr_stage]->src_reg1, instruct_array[EX2_stage]->dst_reg) == 0) {
	 printf("2\n");
	 instruct_array[curr_stage]->has_needed_data = 1;
	 printf("3\n");
       }
       if (strcmp(instruct_array[curr_stage]->src_reg2, instruct_array[EX2_stage]->dst_reg) == 0) {
	 printf("4\n");
	 instruct_array[curr_stage]->has_needed_data2 = 1;
	 printf("5\n");
       }
     }
   }
   if ((curr_stage != EX3_stage) && (EX3_stage >= 0)) {
     if (!(mask & (1 << EX3))) {
       // IF can fwd at this time, and the current instruct depended on it
       if (strcmp(instruct_array[curr_stage]->src_reg1, instruct_array[EX3_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data = 1;
       }
       if (strcmp(instruct_array[curr_stage]->src_reg2, instruct_array[EX3_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data2 = 1;
       }
     }
   }
   if ((curr_stage != EX4_stage) && (EX4_stage >= 0)) {
     if (!(mask & (1 << EX4))) {
       // IF can fwd at this time, and the current instruct depended on it
       if (strcmp(instruct_array[curr_stage]->src_reg1, instruct_array[EX4_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data = 1;
       }
       if (strcmp(instruct_array[curr_stage]->src_reg2, instruct_array[EX4_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data2 = 1;
       }
     }
   }
   if ((curr_stage != ME_stage) && (ME_stage >= 0)) {
     if (!(mask & (1 << ME))) {
       // IF can fwd at this time, and the current instruct depended on it
       if (instruct_array[curr_stage]->src_reg1 == instruct_array[ME_stage]->dst_reg) {
	 instruct_array[curr_stage]->has_needed_data = 1;
       }
       if (instruct_array[curr_stage]->src_reg2 == instruct_array[ME_stage]->dst_reg) {
	 instruct_array[curr_stage]->has_needed_data2 = 1;
       }
     }
   }
   if ((curr_stage != WB_stage) && (WB_stage >= 0)) {
     if (!(mask & (1 << WB))) {
       // IF can fwd at this time, and the current instruct depended on it
       if (strcmp(instruct_array[curr_stage]->src_reg1, instruct_array[WB_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data = 1;
       }
       if (strcmp(instruct_array[curr_stage]->src_reg2, instruct_array[WB_stage]->dst_reg) == 0) {
	 instruct_array[curr_stage]->has_needed_data2 = 1;
       }
     }
   }
 }

     
     
void ForwardAvailableResources(INSTRUCT_T * instruct_array[], int curr_stage, int IF_stage, 
			       int ID_stage, int EX1_stage, int EX2_stage, int EX3_stage, 
			       int EX4_stage, int ME_stage, int WB_stage) {
  printf("FORWARDING ATTEMPT\n");
  int dependencies = GetDependencies(instruct_array, curr_stage, IF_stage, ID_stage, 
				     EX1_stage, EX2_stage, EX3_stage, EX4_stage, 
				     ME_stage, WB_stage);
  printf("dependencies found: %d\n", dependencies);
  // the masks represent where forwarding is available (@0s)
  // wait I have to represent the instruction here too
  // if MULT, i cant fwd unless it is in EX4

  int mask = 0b11111111;
  // check if the data can even be forwarded, 
  // mix mask and the ID\EX1\ME mask with this one

  // if there is a dependency on the IF stage
  if (dependencies & (1 << IF)) {
    printf("IF\n");
    // if it can forward at the stage it is presently, then adjust the mask as so
    // if soonest I can fwd is before or at the  current stage
    if (instruct_array[IF_stage]->soonest_fwd <= IF) {
      printf("IF masking\n");
      mask &= (0 << IF);
    }
  }
  
  if (dependencies & (1 << ID)) {
    printf("ID\n");
    if (instruct_array[ID_stage]->soonest_fwd <= ID) {
      printf("ID masking\n");
      mask &= (0 << ID);
    }
  }

  if (dependencies & (1 << EX1)) {
    printf("EX1\n");
    if (instruct_array[EX1_stage]->soonest_fwd <= EX1) {
      printf("EX1 masking\n");     
	mask &= (0 << EX1);
    }
  }

  if (dependencies & (1 << EX2)) {
    printf("EX2\n");
    if (instruct_array[EX2_stage]->soonest_fwd <= EX2) {
      printf("EX2 masking\n");
      mask &= (0 << EX2);
    }
  }

  if (dependencies & (1 << EX3)) {
    printf("EX3\n");
    if (instruct_array[EX3_stage]->soonest_fwd <= EX3) {
      printf("EX3 masking\n");
      mask &= (0 << EX3);
    }
  }

  if (dependencies & (1 << EX4)) {
    printf("EX4\n");
    if (instruct_array[EX4_stage]->soonest_fwd <= EX4) {
      printf("EX4 masking\n");
      mask &= (0 << EX4);
    }
  }

  if (dependencies & (1 << ME)) {
    printf("ME\n");
    if (instruct_array[ME_stage]->soonest_fwd <= ME) {
      printf("ME masking\n");
      mask &= (0 << ME);
    }
  }
  
if (dependencies & (1 << WB)) {
    printf("WB\n");
    if (instruct_array[WB_stage]->soonest_fwd <= WB) {
      printf("WB masking\n");
      mask &= (0 << WB);
    }
  }

  int ID_mask =  0b10100011;
  int EX1_mask = 0b10100111;
  int ME_mask =  0b10111111;

 printf("mask: %d\n",mask);

// shift the masks over since i say that the forwarding occurs after the stage?
/*
  int ID_mask =  0b01000111;
  int EX1_mask = 0b01001111;
  int ME_mask =  0b01111111;
*/
  // need to check for the most recent instruction dependency.
  // 

  int full_mask;


  // Forwarding is only available at certain stages
  if (curr_stage == ID_stage) {
    full_mask = ID_mask | mask;
    // full mask tells me what can fwd at this time
  }
  else if (curr_stage == EX1_stage) {
    full_mask = EX1_mask | mask;
  }
  else if (curr_stage == ME_stage) {
    full_mask = ME_mask | mask;
  }
  else {
    full_mask = 0b11111111;
  }
  
  SetHasNeeded(instruct_array, full_mask, curr_stage, IF_stage, ID_stage, 
	       EX1_stage, EX2_stage, EX3_stage, EX4_stage, 
	       ME_stage, WB_stage);
}
