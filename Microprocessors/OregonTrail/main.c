// main.c
#include "map.h"
#include "threat.h"
#include "game.h"
#include "unistd.h" // for file checking


int main(int argc, char* argv[]) {
  int food, viewPortSize, destroyed;
  SPACE * currentLocation;
  SPACE * nextLocation;
  
  char * filename;
  
  if (argc > 1) {
    int count = 0;
    filename = argv[1];
    if (access(filename, F_OK) != -1) {
    
      FILE* file = fopen(filename, "r");
      char line[256];
      
      while (fgets(line, sizeof(line), file)) {
	if (count == 1) { // handle view port
	  if (line[10] == 10 || line[10] == 0) { // viewport is in 1's
	    viewPortSize = (line[9] - 48);
	  }
	  else { // viewport is in 10's
	    viewPortSize = (line[9] - 48) * 10 + (line[10] - 48);
	  }
	}
	else if (count == 2) { // handle food
	  if (line[7] != 10 && line[7] != 0) { // food is in the 100's
	    food = (line[5] - 48) * 100 + (line[6] - 48) * 10 + (line[7] - 48);
	  }
	  else if (line[6] != 10 && line[6] != 0) { // food is in the 10's
	    food = (line[5] - 48) * 10 + (line[6] - 48); 
	  }
	  else{ // food is in the 1's
	    food = line[5] - 48;
	  }
	}
	count++;
      }
      fclose(file);
    }
    else {
      printf("Invalid filename ... Quitting ...\n\n");  
      return 0;
    }
  }
  else {
    printf("Invalid input ... Quitting ... \n\n");
    return 0;
  }
  
  char userInput;
  userInput = DisplaySplash();
  if (userInput == 'i') {
    DisplayInstructions();
  }
  
  else if (userInput == 's') {
    SPACE *start;
    start = ConstructMap(filename);
    currentLocation = start;
    
    while (currentLocation->mapChar != 'Z') {
      DisplayViewPort(currentLocation, viewPortSize);
      printf("Remaining Food: %d\n\n", food);
      
      if (food <= 0) {
	printf("\n--- YOU LOSE ---\n\n");
	printf("Destroying map ...\n");
	destroyed = DeleteMap(&start);
      if (destroyed == 1) {
	printf("Destruction Success. Goodbye!\n");
      }
      else {
	printf("Destruction failed. ERROR.\n");
      }
	return 0;
      }
      userInput = UserMove(currentLocation);
      
      if (userInput == 'l') {
	nextLocation = currentLocation->left;
	nextLocation->discovered = 1;
	DisplayViewPort(currentLocation, viewPortSize);
	currentLocation->discovered = 1;
      }    
      
      else if (userInput == 'r') {
	nextLocation = currentLocation->right;
	nextLocation->discovered = 1;
	DisplayViewPort(currentLocation, viewPortSize);	
	currentLocation->discovered = 1;
      }
      
      else if (userInput == 'u') {
	nextLocation = currentLocation->up;
	nextLocation->discovered = 1;
	DisplayViewPort(currentLocation, viewPortSize);
	currentLocation->discovered = 1;
      }
      
      
      else if (userInput == 'd') {
	nextLocation = currentLocation->down;
	nextLocation->discovered = 1;
	DisplayViewPort(currentLocation, viewPortSize);
	nextLocation->discovered = 1;
      }
      
      else {
	printf("Invalid Input ... Quitting ...\n");
      }
      
      if (nextLocation->mapChar == 'O') {
	printf("The ocean isn't really the place for you. Don't you think?\n");
      }
      else {
	int foodChange = Encounter(nextLocation);
	if (foodChange == -1) {
	  //dont change location;
	}
	
	else {
	  food = food - foodChange;
	  currentLocation = nextLocation;
	}  
      }
    }
    if (currentLocation->mapChar == 'Z') {
      printf("\n~~~** WINNER **~~~\n\n");
            
      printf("Destroying map ...");
      destroyed = DeleteMap(&start);
      if (destroyed == 1) {
	printf("Destruction Success. Goodbye!\n");
      }
      else {
	printf("Destruction failed. ERROR.\n");
      }
    }
  }
  else if (userInput == 'q') {
    printf("Quitting ...\n");
    return 0;
  }
  
  
  return 0;
}
