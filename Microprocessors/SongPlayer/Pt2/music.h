#ifndef _MUSIC_H
#define _MUSIC_H


#include <inttypes.h>
#include <ctype.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "U0_UART.h"
#include <util/delay.h>
#include <avr/eeprom.h>

#define NOTE_A 0
#define NOTE_B 1
#define NOTE_C 2
#define NOTE_D 3
#define NOTE_E 4
#define NOTE_F 5
#define NOTE_G 6
#define NOTE_R 7

#define PORTB5_SPEAKER_MASK 0b00100000
#define USER_LINE_MAX 128
#define NUMBER_OF_SONGS 4
#define MAX_SONG_LENGTH 64
#define MAX_TITLE_LENGTH 32

#define FREQ_A4_HZ 440
#define HALFPERIOD_A4_US 1136
#define FREQ_B4_HZ 494
#define HALFPERIOD_B4_US 1012
#define FREQ_C3_HZ 262
#define HALFPERIOD_C3_US 1912
#define FREQ_D3_HZ 294
#define HALFPERIOD_D3_US 1700
#define FREQ_E3_HZ 330
#define HALFPERIOD_E3_US 1516
#define FREQ_F3_HZ 350
#define HALFPERIOD_F3_US 1432
#define FREQ_G3_HZ 392
#define HALFPERIOD_G3_US 1276 //delay in microseconds



void PlayNote(uint8_t letterASCII, uint8_t quarters);

void PlaySong(uint8_t song[]);

uint8_t PackNote(char letterASCII, uint8_t duration);

uint8_t UnpackNoteLetterASCII(uint8_t packedNote);

uint8_t UnpackNoteDuration(uint8_t packedNote);

void StoreSong(uint8_t song[], const char songString[]);

void StripEOL(char string[],int n); // what is this for?

uint8_t DisplayMenu(const char menu[]);

void SelectSong(uint8_t menuSelection, char songTitle[NUMBER_OF_SONGS][MAX_TITLE_LENGTH], uint8_t song[]);// implement this

void CreateSong(uint8_t eraseChoice , char songTitle[NUMBER_OF_SONGS][MAX_TITLE_LENGTH], uint8_t song[]);

void ListSongs(char songTitles[NUMBER_OF_SONGS][MAX_TITLE_LENGTH]);

uint8_t MatchScore(const char countQueryString[], const char database[]);

void delay_microseconds(int us);



#endif