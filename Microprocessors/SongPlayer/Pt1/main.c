/*
 * main.c
 *
 * Created: 10/12/2016 6:41:16 PM
 *  Author: saressa1
 */
#include <asf.h>


#include <music.h>

#define PORTB5_SPEAKER_MASK 0b00100000
#define USER_LINE_MAX 128
char userLine[USER_LINE_MAX];
#define NUMBER_OF_SONGS 4
#define MAX_SONG_LENGTH 64
#define MAX_TITLE_LENGTH 128 // ask about this
char songTitle[NUMBER_OF_SONGS][MAX_SONG_LENGTH];
uint8_t songs[NUMBER_OF_SONGS][MAX_SONG_LENGTH];


int main(void)
{
	stderr = stdout = stdin = &uart_stream; 
	UARTInit();
	
// while(1)    {

  char songASCII[] = "A13R5B1D21\n";
  uint8_t songPacked[20];
  StoreSong(songPacked, songASCII);
  PlaySong(songPacked);
// }
	return 0;
}
