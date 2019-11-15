#include "game.h"

char DisplaySplash(void) {
  char userInput;
  int askedOnce = 0;
  do {
    
    printf("--------ORGEON TRAIL--------\n");
    printf("                            \n");
    printf("   s - Start Game           \n");
    printf("   i - Display Instructions \n");
    printf("   q - Quit Game            \n");
    printf("                            \n");
    printf("----------------------------\n");
    
    if (!askedOnce) {
      printf("Please make a selection: ");
      scanf(" %c", &userInput);
      printf("\n");
      askedOnce = 1;
    }
     
    else {
      printf("Invalid Selection. Please try again: ");
      scanf(" %c", &userInput);
      printf("\n");
      
    }

    if (userInput == 's' || userInput == 'S' ||
	userInput == 'i' || userInput == 'I' ||
	userInput == 'q' || userInput == 'Q' ) {
      return tolower(userInput);
    }
    
  } while (userInput != 's' || userInput != 'S' || 
	   userInput != 'i' || userInput != 'I' ||
	   userInput != 'q' || userInput != 'Q' );
  
  return userInput;
}

void DisplayInstructions(void) {
  printf("-------------------------INSTRUCTIONS--------------------------\n");
  printf("                                                               \n");
  printf(" Game Description: You are traversing through a map and have   \n");
  printf("      limited food resources. Encountering various obstacles   \n");
  printf("      will use resources. Your goal is to reach the destination\n");
  printf("      before you exhaust your food resources.                  \n");
  printf("                                                               \n");  
  printf(" Controls: You will type the following characters to control   \n");
  printf("      your movements:                                          \n");
  printf("                                                               \n");
  printf("        up - 'u'   down - 'd'   left - 'l'   right - 'r'       \n");
  printf("                                                               \n");
  printf(" View Port: You will only be able to see a portion of the map  \n");
  printf("      through the view port that will be shown with each       \n");
  printf("      move made. Various letters will represent what is at the \n");
  printf("      space. Arriving at a space will reveal what is there.    \n");
  printf("                                                               \n");
  printf("      X - Your current location                                \n");
  printf("      Z - Your destination                                     \n");
  printf("      U - Undiscovered area                                    \n");
  printf("      O - Ocean which borders the map. You cannot go here.     \n");
  printf("                                                               \n");
  printf(" Obstacles: You will encounter obstacles in the form of        \n");
  printf("      rivers, animals or diseases.                             \n");
  printf("                                                               \n");
  printf(" Rivers: You will be given the option to retreat from or cross \n");
  printf("      the river.                                               \n");
  printf("                                                               \n");
  printf("                 R - River       ->  -20 Food                  \n");
  printf("                                                               \n");
  printf(" Animals: You will be given the option to retreat from or hunt \n");
  printf("       the animal. Hunting results in various food alterations.\n");
  printf("                                                               \n");
  printf("                 G - Grizzly     ->  -10 Food                  \n");
  printf("                 B - Boar        ->  - 5 Food                  \n");
  printf("                 E - Elk         ->    0 Food                  \n");
  printf("                 H - Hare        ->  + 5 Food                  \n");
  printf("                                                               \n");
  printf(" Diseases: Once you step into a disease space, you contract    \n");
  printf("       the disease automatically.                              \n");
  printf("                                                               \n");
  printf("                 C - Cold        ->  - 5 Food                  \n");
  printf("                 F - Flu         ->  -10 Food                  \n");
  printf("                 D - Dysentery   ->  -15 Food                  \n");
  printf("                                                               \n");
  printf(" Winning Condition: You win if you reach the destination before\n");
  printf("       running out of food.                                    \n");
  printf("                                                               \n");
  printf(" Losing Condition: You lose if you run out of food before      \n");
  printf("       reaching your destination.                              \n");
  printf("                                                               \n");
  printf("---------------------------------------------------------------\n");
  return;
}


char UserMove(SPACE * currentLocation) {
  char c;
  SPACE * current = currentLocation;
  int validInput = 0;
  while (validInput == 0) {
    printf("   u - up\n");
    printf("   d - down\n");
    printf("   l - left\n");
    printf("   r - right\n");
    
    
    printf("Your move: ");
    scanf(" %c",&c);
    printf("\n"); 
    
    
    if (c == 'u' || c == 'U' ||
	c == 'd' || c == 'D' ||
	c == 'l' || c == 'L' ||
	c == 'r' || c == 'R' ) {
      
      if (tolower(c) == 'u') {
	if (current->up != NULL) {
	  return tolower(c);
	}
      }
      else if (tolower(c) == 'd') {
	if (current->down != NULL) {
	  return tolower(c);
	}
      }
      
      
      else if (tolower(c) == 'l') {
	if (current->left != NULL) {
	  return tolower(c);
	}
      }
      
      
      else if (tolower(c) == 'd') {
	if (current->down != NULL) {
	  return tolower(c);
	}
      }      
    } 
  }
  
  return tolower(c);
}


int  Encounter(SPACE * nextLocation) {
  // if int = -1 didn't move
  // if int = any number, subtract that later
  char encounter = nextLocation->mapChar;
  nextLocation->discovered = 1;
  char userInput;
  switch (encounter) {
    //River
  case 'R' :
    printf("You are attempting to ford a river.\nYou may either choose to ford(f) or retreat(r): ");
    scanf(" %c", &userInput);
    printf("\n");
    if (userInput == 'f') {
      printf("Fording ... \n");
      return 20;
    }
    
    if (userInput == 'r') {
      printf("Retreating ... \n");
      return -1;
    }
    break;
    
    // Grizzly Bear
  case 'G' :
    if (nextLocation->threat.animal.alive == 1) {
      printf("You have encountered a Grizzly Bear.\nWould you choose to hunt(h) or retreat(r): ");
      scanf(" %c", &userInput);
      printf("\n");
      
      if (userInput == 'h') {
	printf("Hunting ...\n");
	nextLocation->threat.animal.alive = 0;
      }
      return 10;
    
      
      if (userInput == 'r') {
	printf("Retreating ...\n");
	return -1;
      }
    }
    break;
    
    // Boar
  case 'B' :
    if (nextLocation->threat.animal.alive == 1) {
      printf("You have encountered a Boar.\nWould you choose to hunt(h) or retreat(r): ");
      scanf(" %c", &userInput);
      printf("\n");
      if (userInput == 'h') {
	nextLocation->threat.animal.alive = 0;
	printf("Hunting ...\n");
	
	return 5;
      }
      
      if (userInput == 'r') {
	printf("Retreating ...\n");
	return -1;
      }
    }
    break;
    
    // Elk
  case 'E' :
    if (nextLocation->threat.animal.alive == 1) {
      printf("You have encountered an Elk.\nWould you choose to hunt(h) or retreat(r): ");
      scanf(" %c", &userInput);
      printf("\n");
      if (userInput == 'h') {
	printf("Hunting ...\n");
	nextLocation->threat.animal.alive = 0;
	return 0;
      }
      
      if (userInput == 'r') {
	printf("Retreating ...\n");
	return -1;
      }
    }
    break;
    
    // Hare
  case 'H' :
    if (nextLocation->threat.animal.alive == 1) {
      printf("You have encountered a Hare.\nWould you choose to hunt(h) or retreat(r): ");
      scanf(" %c", &userInput);
      printf("\n");
      if (userInput == 'h') {
	printf("Hunting ...\n");
	nextLocation->threat.animal.alive = 0;
	return -5;
      }
      
      if (userInput == 'r') {
	printf("Retreating ...\n");
	return -1;
      }
    }
    break;
    
      
    // Cold
  case 'C' :
    printf("You have contracted the common cold, your food storage drops by 5 as you take a day off to recuperate");
    return 5;
    break;
    
    // Cold
  case 'F' :
    printf("You have contracted the flu, your food storage drops by 10 as you take a day off to recuperate");
    return 10;
    break;
  
    // Dysentery
  case 'D' :
    printf("You have contracted the dysentery, your food storage drops by 15 as you take a day off to recuperate");
    return 15;
    break;
    
    
  default:
    break;  
  }
  
  return 0;
}

