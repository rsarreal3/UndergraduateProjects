/* memory.c */
#include "memory.h"

int InitDataMemory(long data_array[], char* filename){
  FILE* my_file = fopen(filename, "r");
  if (my_file == NULL) {
    printf("ERROR: Could not open memory file.\n");
    return 0;
  }
  
  long data = 0;
  int i = 0;
  int index = 0;
  char line[64];
  char *save;
  while (fgets(line, sizeof(line), my_file)) {
    save = line;
    printf("save = %s\n", save);
    data = 0;
    for (i = 0 ; i <= strlen(save) ; i++) {      
      if ((save[i] == 48) || (save[i] == 49)){
	data = data << 1;
	data |= (save[i] - 48);
      }
    }
    data_array[index] = data;
    index++;
  }
  
  printf("MY MEM DATA ARRAY\n");
  for (i = 0 ; i < 32 ; i++) {
    printf("%d %lx\n", i, data_array[i]);
  }
  printf("\n");
  fclose(my_file);
  return 1;
}



int RegStrToInt(char* reg_str) {
  int reg_int = 0;
  if (reg_str[2] >= 48 && reg_str[2] <= 57) {
    // if the register is 10 - 32
    printf("reg_str 1 = %c\n", reg_str[1]);
    reg_int = (reg_str[1] - 48) * 10 + (reg_str[2] - 48);
  }
  else {
    
    reg_int = (reg_str[1] - 48);
  }
  printf("reg int = %d\n", reg_int);
  return reg_int;
}

int IsInstructionHit(DA_CACHE_T* instruct_cache, long address) {
  long tag_mask = 0xFFF0;
  long index_mask = 0x6;
  long tag = (address & tag_mask) >> 4;
  long index = (address & index_mask) >> 2; 

  if ((instruct_cache->valid[index] == 1) &&
      (instruct_cache->tag[index] == tag)) {
    return 1;
  }
  return 0;
}

long AccessInstructionCache(DA_CACHE_T* instruct_cache, long memory[],
			    long address, int rd_wr, int hit, long data) {
  long tag_mask = 0xFFF0; 
  long tag = address & tag_mask;
  long index_mask = 0x6;
  long index = address & index_mask;
  // assume it is a hit

  if (hit == 0) {
    instruct_cache->block[index] = memory[address]; // address probably wrong
    instruct_cache->tag[index] = tag;
    instruct_cache->valid[index] = 1;
  }
  
  if (rd_wr == 1) {  
    return instruct_cache->block[index];
  }
  
  else {
    instruct_cache->block[index] = data;
  }
  return 0;
}
  
  

int IsDataHit(TWSA_CACHE_T* instruct_cache, long address) {
  long tag_mask = 0xFFF0;
  long index_mask = 0x6;
  long tag = (address & tag_mask) >> 4;
  long index = (address & index_mask) >> 2; 

  if ((instruct_cache->valid[index] == 1) &&
      (instruct_cache->tag[index] == tag)) {
    return 1;
  }
  return 0;
}

long AccessDataCache(TWSA_CACHE_T* instruct_cache, long memory[],
			    long address, int rd_wr, int hit, long data) {
  long tag_mask = 0xFFF0; 
  long tag = address & tag_mask;
  long index_mask = 0x6;
  long index = address & index_mask;
  // assume it is a hit

  if (hit == 0) {
    instruct_cache->block[index] = memory[address]; // address probably wrong
    instruct_cache->tag[index] = tag;
    instruct_cache->valid[index] = 1;
  }
  
  if (rd_wr == 1) {  
    return instruct_cache->block[index];
  }
  
  else {
    instruct_cache->block[index] = data;
  }
  return 0;
}
  
  