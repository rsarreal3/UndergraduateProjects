#ifndef MEMORY_H
#define MEMORY_H

#include"data_struct.h"

typedef struct DA_Cache {
  long block[4];
  int valid[4];
  long tag[4];
}DA_CACHE_T;

typedef struct TWSA_Cache {
  DA_CACHE_T* set[2]; // two sets 
  int lru;
}TWSA_CACHE_T;


int InitDataMemory(long data_array[], char* filename);
int RegStrToInt(char* reg_str);
int IsInstructionHit(DA_CACHE_T* instruct_cache, long address);
long AccessInstructionCache(DA_CACHE_T* instruct_cache, long memory[],
			    long address, int rd_wr, int hit, long data); 


#endif

