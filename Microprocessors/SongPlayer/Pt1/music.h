#ifndef _MUSIC_H
#define _MUSIC_H
//extern FILE uart_stream
#include <avr/io.h>

#include <util/delay.h>
#include <inttypes.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "U0_UART.h"

void PlayNote(uint8_t letterASCII, uint8_t quarters);

void PlaySong(uint8_t song[]);

uint8_t PackNote(char letterASCII, uint8_t duration);

uint8_t UnpackNoteLetterASCII(uint8_t packedNote);

uint8_t UnpackNoteDuration(uint8_t packedNote);

void StoreSong(uint8_t song[], const char songString[]);

#endif