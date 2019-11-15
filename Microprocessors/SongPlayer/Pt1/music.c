/*
 * music.c
 *
 * Created: 10/12/2016 6:41:16 PM
 *  Author: saressa1
 */ 
#include <music.h>


void PlayNote(uint8_t letterASCII, uint8_t quarters) {
	printf("*%c%d*", letterASCII, quarters); // just print the values
	return;
}




void PlaySong(uint8_t song[]) {
	int songLen = 0; 
	uint8_t current;
	while (1) {
			
		songLen++;
		current = song[songLen-1];
		if (current == 0b11100000) {
			break;
		}
	
	}
	//int songLen = sizeof(song); // for every binary representation of a note
	uint8_t noteData;
	uint8_t note_ascii;
	uint8_t duration;
	for (int i = 0 ; i < songLen; i++) {
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
	// the char is the char
	// the duration is the asii char value
	uint8_t binaryVal;
	if (toupper(letterASCII) == 'R') {
		binaryVal = 0b00000111;
	}
	else {
		binaryVal = letterASCII - 65;
	}
	binaryVal = binaryVal << 5;
	binaryVal += (duration);
	
	return binaryVal;
}



void StoreSong(uint8_t song[], const char songString[]){
	uint8_t binaryVal;
	char note;
	uint8_t duration1, duration2, durationNum1, durationNum2;
	uint8_t duration = 0;
	int noteCount = 0;
	int songStrLen = strlen(songString);
	
	for (int i = 0 ; i < songStrLen + 1 ; i++) {
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
			durationNum1 = (int)duration1 - 48;
			durationNum2 = (int)duration2 - 48;
		}
		
		if ((durationNum1 + durationNum2 == 0 && toupper(note) == 'R') || (songString[i] == '\0')) {
			song[noteCount] == 0b11100000;

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
		duration == 0;
	}
	return;
}




uint8_t UnpackNoteLetterASCII(uint8_t packedNote) {
	char noteASCII;
	uint8_t note = packedNote & 0b11100000;

	if (note == 0b11100000) {
		//   noteASCII = 'R'; //R's ASCII Value
		return 'R';
	}
	else {
		uint8_t noteShft = note >> 5;
		noteASCII = noteShft + (char)65;
	}
	return noteASCII;
}



uint8_t UnpackNoteDuration(uint8_t packedNote) {
	uint8_t duration = packedNote & 0b00011111;
	return duration;
}







