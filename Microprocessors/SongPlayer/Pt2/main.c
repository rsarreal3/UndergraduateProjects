/*
 * main.c
 *
 * Created: 10/12/2016 6:41:16 PM
 *  Author: saressa1
 */
#include <asf.h>
#include <music.h>
uint8_t song[NUMBER_OF_SONGS][MAX_SONG_LENGTH] = {{(NOTE_E << 5)+2 ,(NOTE_D << 5)+2,(NOTE_C<<5) +2 },{NOTE_R<<5},{NOTE_R<<5},{(NOTE_C<<5) +3}};
char songTitle [NUMBER_OF_SONGS][MAX_TITLE_LENGTH]={"Title1","Title2","Title3","Title4"};
const char mainMenu[]  = "Main Menu\nCreate Song\nPlay Song\nList Songs\n";
const char songMenu[] = "Play Song Menu\nPlay by Number\nSearch By Title\n";

int main(void) {
	board_init();
	stderr = stdout = stdin = &uart_stream; 
	UARTInit();

	uint8_t userSelect;
	/*
 while(1)    {
	userSelect = DisplayMenu(mainMenu);

 if ((userSelect - 48) == 1) {
	 printf_P(PSTR("Enter the song number to replace: "));
	userSelect = getc(stdin);
	printf_P(PSTR("\n"));
   if (userSelect - 48 >=0 && userSelect - 48 <= 3) {
     CreateSong(userSelect - 48, songTitle, song);
   }
 }

 
	else if ((userSelect - 48) == 2) {
	userSelect = DisplayMenu(songMenu);
	printf_P(PSTR("%d"), userSelect);
    SelectSong(userSelect - 48, songTitle, song);
}
  else if ((userSelect - 48) == 3) {
   ListSongs(songTitle);
 }
 
 else {
	printf_P(PSTR("DONE\n"));

 }
 }*/
	
PlayNote('A', 4);
  return 0;

}