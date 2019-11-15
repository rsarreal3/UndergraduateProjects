#ifndef _GAME_H
#define _GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "map.h"

char DisplaySplash();

void DisplayInstructions();

char UserMove(SPACE * currentLocation);

int Encounter(SPACE * currentLocation);



#endif
