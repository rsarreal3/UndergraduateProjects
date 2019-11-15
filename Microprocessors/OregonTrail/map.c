//map.c

#include "map.h"
int mapSize;
int viewPortSize;


typedef struct map { 
  SPACE *start;
} MAP;

SPACE * CreateNode(char mapChar, int xCoord, int yCoord) {
  SPACE * newNode;
  newNode = (SPACE *) malloc(sizeof(SPACE));
  if (newNode == NULL) {
    printf("ERROR: Memory Full. Exiting ...");
    return NULL;
  }

  newNode->right = NULL;
  newNode->left = NULL;
  newNode->up = NULL;
  newNode->down = NULL;
  newNode->discovered = 0;
  newNode->xLocation = xCoord;
  newNode->yLocation = yCoord;
  newNode->mapChar = mapChar;
  newNode->playerHere = 0;

  switch (mapChar) {
  case 'G' :
    newNode->threat.animal.foodChange = -10;
    newNode->threat.animal.alive = 1;
    break;

  case 'B' :
    newNode->threat.animal.foodChange = -5;
    newNode->threat.animal.alive = 1;
    break;

  case 'E' :
    newNode->threat.animal.foodChange = 0;
    newNode->threat.animal.alive = 1;
    break;

  case 'H' :
    newNode->threat.animal.foodChange = 5;
    newNode->threat.animal.alive = 1;
    break;

  case 'C' :
    newNode->threat.disease.foodChange = -5;
    break;

  case 'F' :
    newNode->threat.disease.foodChange = -10;
    break;

  case 'D' : 
    newNode->threat.disease.foodChange = -15;
    break;

  default :
      break;
  }  

  return newNode;
}

int DeleteMap(SPACE ** start) {
  SPACE * myNode;
  SPACE * below;
  SPACE * temp;
 
  if (*start == NULL) { // the node was already empty
    return 1;
  }
  
  myNode = *start;
  //finding the upper left corner of the map for the start of destruction
  while (1 == 1) {
    if (myNode->up == NULL ) {
      break;
    }
    else {
      myNode = myNode->up;
    }
  }
  
  while(1 == 1) {
    if (myNode->left == NULL) {
      break;
    }
    else {
      myNode = myNode->left;
    }
  }
 
  // free all nodes in the map
  while (1 == 1) {
    if (myNode == NULL) {
      return 1;
      break;
    }
    below = myNode->down;

    while (myNode != NULL){ 
      temp = myNode;
      myNode = myNode->right;
      free(temp);
    }	
    myNode = below;
  }
  return 0;
}


SPACE * ConstructMap(const char filename[]) {
  FILE* file = fopen(filename, "r");
  char line[256];
  int count = 0;
 
  int fullMapSize;
  SPACE *holdAboveStart;
  SPACE *lSide;
  SPACE *above;
  SPACE *start;
  SPACE *newSpace;
  
  while (fgets(line, sizeof(line), file)) {
    if (count == 0) {
      if (line[9] != 0 && line[9] != 10) {  // mapsize is in the 10's
	mapSize = (line[8] - 48) * 10 + (line[9] - 48);
      }
      else{ // mapsize is in the 1's
	mapSize = line[8] - 48; 
      }
      fullMapSize = mapSize + 2; // must add 2 because mapsize excludes ocean
    }
    else if (count == 1) {
      viewPortSize = line[9] - 48;
    }

    else if (count >= 5) {
      // store values in the actual map
      for (int i = 0 ; i < fullMapSize ; i++ ){ // for every space in the rows
	newSpace = CreateNode(line[i], i, count - 5); // assume origin at top left

	if (count - 5 == 0) { // the start of the map
	  //there will be no top connections. all top should be null
	  if (i == 0) {

	    holdAboveStart = newSpace;
	    lSide = newSpace;
	    newSpace->left = NULL;
	    newSpace->up = NULL;
	    if (line[i] == 'X') {
	      start = newSpace;
	    }
	  }
	  else if ( i == fullMapSize - 1) {
	    newSpace->up = NULL;
	    newSpace->right = NULL;
	    newSpace->left = lSide;
	    lSide->right = newSpace;
	    if (line[i] == 'X') {
	      start = newSpace;
	      newSpace->discovered = 1;
	    }
	  }
	  else {
	    newSpace->up = NULL;
	    newSpace->left = lSide;
	    lSide->right = newSpace;
	    lSide = newSpace;
	    if (line[i] == 'X') {
	      start = newSpace;
	      newSpace->discovered = 1;
	    }
	  }
      	}
	else if (count - 5 == fullMapSize - 1) { // if the space is on the last row
	  // there will be no more bottom. end of file
	  if (i == 0) { // first item in the row
	    above = holdAboveStart;
	    lSide = newSpace;
	    newSpace->left = NULL;
	    newSpace->down = NULL;
	    newSpace->up = above;
	    printf("\n");
	    above->down = newSpace;
	    above = above->right;
	    if (line[i] == 'X') {
	      start = newSpace;
	      newSpace->discovered = 1;
	    }
	  }
	  else if ( i == fullMapSize - 1) { // last item in the row
	    newSpace->up = above;
	    above->down = newSpace;
	    newSpace->right = NULL;
	    newSpace->down = NULL;
	    newSpace->left = lSide;
	    lSide->right = newSpace;
	    // no above = above->right because the previous right is the final 
	    if (line[i] == 'X') {
	      start = newSpace;
	      newSpace->discovered = 1;
	    }
	  }
	  else { // item in the middle of the row
	    newSpace->down = NULL;
	    newSpace->up = above;
	    above->down = newSpace;
	    newSpace->left = lSide;
	    lSide->right = newSpace;
	    lSide = newSpace; // update left side as this new space
	    above = above->right;
	    if (line[i] == 'X') {
	      start = newSpace;
	      newSpace->discovered = 1;
	    }
	  }
	}
	else { // if the space isn't in the first or the last row
	  if (i == 0) { // first item in the row
	    above = holdAboveStart;
	    holdAboveStart = newSpace;
	    newSpace->left = NULL;
	    newSpace->up = above;
	    above->down = newSpace;
	    above = above->right;
	    lSide = newSpace;
	    if (line[i] == 'X') {
	      start = newSpace;
	      newSpace->discovered = 1;
	    }
	  }
	  else if (i == fullMapSize - 1) { // last item in the row
	    newSpace->up = above;
	    newSpace->right = NULL;
	    newSpace->left = lSide;
	    lSide->right = newSpace;
	    if (line[i] == 'X') {
	      start = newSpace;
	      newSpace->discovered = 1;
	    }
	  }	
	  else { // in the middle of the row
	    newSpace->up = above;
	    above->down = newSpace;
	    newSpace->left = lSide;
	    lSide->right = newSpace;
	    lSide = newSpace;
	    above = above->right;
	    if (line[i] == 'X') {
	      start = newSpace;
	      newSpace->discovered = 1;
	    }
	  }
	}
      }
    }
    count++;
  }
  return start;
}

  
void DisplayViewPort(SPACE * userLocation, int viewSize) {
  int viewRadius = (viewSize - 1) / 2;
  SPACE * current = userLocation;

  // Coordinate pair of user's current location
  int xcoord = current->xLocation;
  int ycoord = current->yLocation;

  
  // Coordinate pair of the start of the view port, top left
  int xStart = xcoord - viewRadius;
  int yStart = ycoord - viewRadius;
  //  printf("Display xstart %d\nDisplay ystart %d\n", xStart, yStart);

  // Coordinate pair of the end of the view port, bottom right
  int xEnd = xcoord + viewRadius;
  int yEnd = ycoord + viewRadius;

  // the start of the ocean border
  int upOceanCoord, downOceanCoord, leftOceanCoord, rightOceanCoord;
  
  // find the border coordinates
  while (current->up != NULL){
    current = current->up;
  }
  upOceanCoord = current->yLocation;
  current = userLocation;
  while (current->down != NULL){
    current = current->down;
  }
  downOceanCoord = current->yLocation;
  current = userLocation;
  while (current->left != NULL){
    current = current->left;
  }
  leftOceanCoord = current->xLocation;
  current = userLocation;
  while (current->right != NULL){
    current = current->right;
  }
  rightOceanCoord = current->xLocation;


  current = userLocation;
  printf("\n");
  for (int y = yStart ; y <= yEnd ; y++) {
    printf("\n");

    for (int x = xStart ; x <= xEnd ; x++) {

      if (x <= leftOceanCoord || x >= rightOceanCoord ||
	  y >= downOceanCoord || y <= upOceanCoord) {
	printf("O");
      }
      else { 
	// move to the space, because you are able to
	// if that space is discovered, print the char
	// if not, then print 'U' I think
	int xDist = x - xcoord;
	int yDist = y - ycoord;

	/* the  coordinate system is that 0,0 is the upper left corner */
	if (xDist == 0 && yDist == 0) {
	  printf("X");
	}
	
	else {
	  if (xDist < 0 ) { 
	    for (int i = 0; i > xDist ; i-- ) {
	      current = current->left;
	    }
	  }
	  
	  
	  else if (xDist > 0) {
	    for (int i = 0; i < xDist ; i++ ) {
	      current = current->right;
	    }
	  }
	  
	  
	  if (yDist > 0) {
	    for (int i = 0; i < yDist ; i++ ) {
	      current = current->down;
	    }
	  }
	  
	  else if (yDist < 0){
	    for (int i = 0; i > yDist ; i-- ) {
	      current = current->up;
	    }
	  }
	  if (current->mapChar == 'R') {
	    printf("R");
	  }
	  else if (current->discovered == 0) {
	    printf("U");
	  }
	  else if(current->mapChar == 'C' ||
		  current->mapChar == 'F' ||
		  current->mapChar == 'D') {
	    printf("%c", current->mapChar);
	  } 
	  else if (current->threat.animal.alive == 0) {
	    printf("N");
	  }
	  else { 
	    printf("%c",current->mapChar);
	  }
	  current = userLocation;
	}
      } 
    }
  }
  
  printf("\n");
  return;
}
