/*
 * music.c
 *
 * Created: 10/12/2016 6:41:16 PM
 *  Author: saressa1
 */ 
#include "music.h"
int i;

void PlayNote(uint8_t letterASCII, uint8_t quarters) {
	DDRB = PORTB5_SPEAKER_MASK;
	uint8_t numIterations;
	int halfPeriodMicroseconds;

	if (letterASCII == 'A'){
		numIterations = (FREQ_A4_HZ/4) * quarters;
		halfPeriodMicroseconds = HALFPERIOD_A4_US;
	}
	else if (letterASCII == 'B') {
		numIterations = (FREQ_B4_HZ/4) * quarters;
		halfPeriodMicroseconds = HALFPERIOD_B4_US;
	}
	else if (letterASCII == 'C') {
		numIterations = (FREQ_C3_HZ/4) * quarters;
		halfPeriodMicroseconds = HALFPERIOD_C3_US;
	}
	else if (letterASCII == 'D') {
		numIterations = (FREQ_D3_HZ/4) * quarters;
		halfPeriodMicroseconds = HALFPERIOD_D3_US;
	}
	else if (letterASCII == 'E') {
		numIterations = (FREQ_E3_HZ/4) * quarters;
		halfPeriodMicroseconds = HALFPERIOD_E3_US;
	}
	else if (letterASCII == 'F') {
		numIterations = (FREQ_F3_HZ/4) * quarters;
		halfPeriodMicroseconds = HALFPERIOD_F3_US;
	}
	else if (letterASCII == 'G') {
		numIterations = (FREQ_G3_HZ/4) * quarters;
		halfPeriodMicroseconds = HALFPERIOD_G3_US;
	}
	else {
		PORTB = 0b00000000;
		for (i = 0 ; i < quarters ; i++) {
			_delay_ms(250);
		}
	}
	
	for ( i = 0 ; i < numIterations ; i++) {
		PORTB = 0b00000000; //off
		delay_microseconds(halfPeriodMicroseconds);
		PORTB = 0b00100000; //on
		delay_microseconds(halfPeriodMicroseconds);
		PORTB = 0b00000000;
	}

	//	printf_P(PSTR("*%c%d*"), letterASCII, quarters); // just print the values
	return;
}



void PlaySong(uint8_t song[]) {
	uint8_t songLen = 0; 
	uint8_t current;
	while (1) {	
		songLen++;
		current = song[songLen-1];
		if (current == 0b11100000) {
			break;
		}
	}
	uint8_t noteData;
	uint8_t note_ascii;
	uint8_t duration;
	for (i = 0 ; i < songLen; i++) {
		noteData = song[i];
		note_ascii = UnpackNoteLetterASCII(noteData); // get note letter
		duration = UnpackNoteDuration(noteData); //get the duration
		PlayNote(note_ascii, duration); //Play the note
		if (noteData == 0b11100000) { // if the note was R0, stop playing notes
			break;
		}
	}
	return;
}



uint8_t PackNote(char letterASCII, uint8_t duration) { //fixed
	if (toupper(letterASCII) == 'R') {
		return ((NOTE_R << 5) + duration);
	}
	else {
		return (((letterASCII - 65) << 5) + duration);
	}
}



void StoreSong(uint8_t song[], const char songString[]){
	char note;
	uint8_t duration1, duration2, durationNum1, durationNum2;
	uint8_t duration = 0;
	uint8_t noteCount = 0;
	uint8_t songStrLen = strlen(songString);
	
	for (i = 0 ; i < songStrLen + 1 ; i++) {
		duration = 0;
		durationNum2 = 0;
		if (songString[i] == '\0' || songString[i+1] == '\0') {
			song[noteCount] = 0b11100000;
			break;
		}		
		if (songStrLen - i >= 2) {
			note = songString[i];
			duration1 = songString[i + 1]; // durations are characters
			duration2 = songString[i + 2]; // even if the string is exactly 2 char left, this means that this will be null
			durationNum1 = (uint8_t)duration1 - 48;
			durationNum2 = (uint8_t)duration2 - 48;
		}
		if ((durationNum1 + durationNum2 == 0 && toupper(note) == 'R') || (songString[i] == '\0')) {
		  song[noteCount] = 0b11100000;

			break;
		}
		// only condition for having two digits in the duration if the notes are valid notes, if the duration 2 is still an integer we have a set of 3 values
		if ((((note >= 65) && (note <= 71)) || (note == 82)) || (((note >= 97) && (note <= 103)) || (note == 114))) {		
			if  ((duration2 >= 48) && (duration2 <= 57)) { // if the duration is an integer and the note is in the notes place

				duration = 10 * durationNum1 + durationNum2;
				song[noteCount] = PackNote(note, duration);
				noteCount++;
			}
			else if ((duration2 < 48) || (duration2 > 57)) { // this is if the 3rd value is not an int
				duration = durationNum1;
				song[noteCount] = PackNote(note, duration);
				noteCount++;
			}
		}
		duration = 0;
	}
	return;
}




uint8_t UnpackNoteLetterASCII(uint8_t packedNote) {
	if ((packedNote & 0b11100000) == 0b11100000) {
		//   noteASCII = 'R'; //R's ASCII Value
		return 'R';
	}
	else {
		return (((packedNote & 0b11100000) >> 5) + (char)65);
	}
}



uint8_t UnpackNoteDuration(uint8_t packedNote) {
	return (packedNote & 0b00011111);
}


///////////////////////////////////////////////////// Pt 2

void StripEOL(char string[],int n) {
	for ( i = 0; i < n ; i++) {
		if (string[i] == '\n' || string[i] == '\r') {
			string[i] = '\0';
		}
	}
}


uint8_t DisplayMenu(const char menu[]){
	char inputMenu[strlen(menu)];
	strcpy(inputMenu, menu);
	char *token;
	char s[2] = "\n";
	token = strtok(inputMenu, s);
	uint8_t numTok = 0;
	uint8_t input;
	
	do {
	while(token != NULL) {
	    if ( numTok == 0 ) {
		    printf_P(PSTR("----%s----\n"),  token);
	    }
	    else {
		    printf_P(PSTR("%d.   %s\n"), numTok, token);
	    }
	token = strtok(NULL,s);
	numTok++;
	}
	  printf_P(PSTR("Please Enter Choice: "));
	   input = getc(stdin);

	  printf_P(PSTR("\n"));
	} while (input >= (48 + numTok) || input <= 48);
	return input;
}




  void SelectSong(uint8_t menuSelection, char songTitle[NUMBER_OF_SONGS][MAX_TITLE_LENGTH], uint8_t song[]) {
    uint8_t matchNum = 0;
    uint8_t matchIndex;
    char titleInput[USER_LINE_MAX];

    if (menuSelection == 1) {
	printf_P(PSTR("Please select a song from the list: "));
	uint8_t select = getc(stdin);
	printf_P(PSTR("\n%d\n"),  select);
    PlaySong(song[select - 48]);
    }
	else {
	  // user is prompted a string. string is passed to matchScore with all the
	  // stored titles. If all return 0 -> just play the first one :P
	  printf_P(PSTR("Enter title search string(use space between words): "));

		i = 0;
		char c = getc(stdin);
		titleInput[i] = c;
		i++;
	 while (c >= 32 && c < 127) {
	    c = getc(stdin);	
		titleInput[i] = c;
		i++;	
	}
		printf_P(PSTR("\n"));
	  for (i = 0 ; i < NUMBER_OF_SONGS ; i++) {
	    if (matchNum < (MatchScore(titleInput, songTitle[i]))) {
	      matchNum = MatchScore(titleInput, songTitle[i]);
	      matchIndex = i;
	    }
	  }
		if (matchNum != 0) {
		 PlaySong(song[matchIndex]);
		}
	} 
    return;
  }
  

void CreateSong(uint8_t eraseChoice, char songTitle[NUMBER_OF_SONGS][MAX_TITLE_LENGTH], uint8_t song[]) {
  printf_P(PSTR("Enter Title of Song (may include spaces): "));
  i = 0;
  char c = 'c';
  while (c >= 32 && c < 127) {
    c = getc(stdin);
	songTitle[eraseChoice][i] = c;
	i++;
  }
  printf_P(PSTR("\n"));
  printf_P(PSTR("Enter Song ABCDEFGR followed by quarter seconds: "));
  i = 0;
  char inputString[MAX_SONG_LENGTH];
  c = 'c';
  while (c >= 32 && c < 127) {
	c = getc(stdin);
	inputString[i] = c;
	i++;
}
  printf_P(PSTR("\ns"));
 // printf_P(PSTR("\n"));
 StripEOL(inputString, strlen(inputString));
  StoreSong(song[eraseChoice],inputString); 
  return;
}


 void ListSongs(char songTitles[NUMBER_OF_SONGS][MAX_TITLE_LENGTH]){
   printf_P(PSTR("----Song List---- \n"));
   for (i = 0 ; i < NUMBER_OF_SONGS ; i++) {
	printf_P(PSTR(" %d: Title: %s\n"), i, songTitles[i]);
   }
   return;
}



uint8_t MatchScore(const char countQueryString[], const char database[]){
	uint8_t numMatches = 0;
	char s[2] = " ";
	char *token = strtok(countQueryString, s);
	char *needle = token;
	char *nonExist = NULL;
	const char *haystack;
	while(token != NULL) {
		*needle = token;
		haystack = database;

		if (strstr(haystack, needle) != '\0') {
			numMatches++;
		}
		token = strtok(NULL, s);
	}
	return numMatches;
}

void delay_microseconds(int us) {
	for (i = 0 ; i < us ; i++ ) {
		_delay_us(1);
	}
}