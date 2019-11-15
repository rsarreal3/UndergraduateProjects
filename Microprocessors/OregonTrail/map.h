#ifndef _MAP_H
#define _MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "threat.h"

typedef struct space{ // malloc the nodes, then when i need to destroy the map, free loop to clear
  struct space *right;
  struct space *left;
  struct space *up;
  struct space *down;
  int discovered;
  int xLocation;
  int yLocation;
  char mapChar;
  int playerHere;
  union {
    struct a{
      int foodChange;
      int alive; // 0 or 1 for bool  
    } animal;
    
    
    struct d{
      int foodChange;
    } disease;
  } threat;

  
  enum {Disease, Animal}NUM;
  
} SPACE;


SPACE *  CreateNode(char mapChar, int xCoord, int yCoord);

int DeleteMap(SPACE ** start);

// The map will save and hold data regarding what the spaces hold
SPACE * ConstructMap(const char filename[]);


SPACE *  CreateViewPort(SPACE * userLocation, int viewSize);

// The viewport will not save data. It will only get data from the map.
void DisplayViewPort(SPACE * userLocation , int viewSize); 

#endif
