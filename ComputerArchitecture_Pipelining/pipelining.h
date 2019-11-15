/* File: pipelining.h
 * Author: Ressa Reneth Sarreal
 * Date: 11/14/2017
 * E-mail: saressa1@umbc.edu
 */

#ifndef PIPELINING_H
#define PIPELINING_H

#include "data_struct.h"




int GetSoonestFwd(const char* opcode);
int GetRequiredStage(const char* opcode);


int IsDependent(INSTRUCT_T * instruct_array[], int curr_stage, int IF_stage, int ID_stage,
		int EX1_stage, int EX2_stage, int EX3_stage, int EX4_stage, int ME_stage, 
		int WB_stage);

int CanFwd(INSTRUCT_T * instruct_array[], int curr_stage, int IF_stage, int ID_stage,
		int EX1_stage, int EX2_stage, int EX3_stage, int EX4_stage, int ME_stage, 
		int WB_stage);

int GetDependencies(INSTRUCT_T * instruct_array[], int curr_stage, int IF_stage, int ID_stage,
		int EX1_stage, int EX2_stage, int EX3_stage, int EX4_stage, int ME_stage, 
		int WB_stage);

void SetHasNeeded(INSTRUCT_T * instruct_array[], int mask, int curr_stage, int IF_stage, 
		 int ID_stage, int EX1_stage, int EX2_stage, int EX3_stage, int EX4_stage, 
		 int ME_stage, int WB_stage);

void ForwardAvailableResources(INSTRUCT_T* instruct_array[], int curr_stage, int IF_stage, int ID_stage,
		int EX1_stage, int EX2_stage, int EX3_stage, int EX4_stage, int ME_stage, 
		int WB_stage);
#endif
