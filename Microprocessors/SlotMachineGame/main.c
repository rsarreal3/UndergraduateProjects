//set F_CPU if not set already, dependent on use of boot loader
#ifndef F_CPU
#ifdef USING_BOOTLOADER
#define F_CPU 2000000UL
#else
#define F_CPU 8000000UL
#endif
#endif

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "U0_LCD_Driver.h"

//////////////
//globals   //
//////////////
#define WINNING_SCORE 850
static int currentScreen = 1; // can be only 1 or 2
static int totalScore = 0;
static int remainingTurns = 10;
static int SpinSlot = 0; // 0 = False, 1 = True



//////////////
//prototypes//
//////////////
void SetupInterrupts(void);
void BootLoaderFixes(void);
void DisplayScore(int value);
void DisplayTurnsRemaining(int turns);
void LoseBuzzer(void);
void WinBuzzer(void);
void WinLight(void);

////////
//MAIN//
////////
int main(void){

	#ifdef USING_BOOTLOADER
	BootLoaderFixes();
	#endif
	/* Initialize and Clear the LCD */
	LCD_Init();

	//Setup All Pushbuttons
	DDRB  &= ~0b11010000;  //set B4,B6,B7 as inputs
	DDRE  &= ~0b00001000;  //set all to inputs

	PORTB |=  0b11010000;  //enable pull up resistors on B4,B6,B7
	PORTE |=  0b00001100;  //enable pull up resistor on pin E2,E3

	//Setup Output for Music
	DDRB |= (1<<5); 	//for the buzzer
	DDRB |= (1<<0);		//for the light
	
	SetupInterrupts();	//setup the interrupts
	sei();				//enable global interrupts

	while(1){
		
		int hundreds = 0;
		int tens = 0;
		int ones = 0;
		int delay_counter = 1;
		int round_score = -1;
		
		
		if (currentScreen == 1) {
			DisplayTurnsRemaining(remainingTurns%10); // will start at 10 turns, so print 0
			LCD_WriteDigit((char)(hundreds + 48), 3);
			LCD_WriteDigit((char)(tens + 48), 4);
			LCD_WriteDigit((char)(ones + 48), 5);
		}

				
		while(SpinSlot == 1) {
			// break if the person decided they want to stop the slots from spinning
			if (delay_counter % 10 == 0) {
				hundreds = (hundreds % 10) + 1;
				if (currentScreen == 1) {
					LCD_WriteDigit((char)(hundreds + 48), 3);
				}
			}
			if (delay_counter % 15 == 0) {
				tens = (tens % 10) + 1;
				if (currentScreen == 1) {
					LCD_WriteDigit((char)(tens + 48), 4);
				}
			}
			if (delay_counter % 20 == 0) {
				ones = (ones % 10) + 1;
				if (currentScreen == 1) {
					LCD_WriteDigit((char)(ones + 48), 5);
				}
			}

			if (SpinSlot == 0 ) { break; }
			if (delay_counter % 5 != 0) { // only here for initial case, before any delay, so we dont increment too soon
				delay_counter = 0;
			}
			if (SpinSlot == 0 ) { break; }
			_delay_ms(5);
			delay_counter += 5;
			if (SpinSlot == 0 ) { break; }
		}
		
		if (SpinSlot == 0) {
			if (currentScreen == 1) {
			LCD_WriteDigit((char)(hundreds + 48), 3);
			LCD_WriteDigit((char)(tens + 48), 4);
			LCD_WriteDigit((char)(ones + 48), 5);
			}
		}
		round_score = (hundreds * 100) + (tens * 10) + ones;
		if (round_score != -1) { // if roundscore == -1 -> we haven't spun yet
			totalScore = (totalScore + round_score) % 1000;
			if (currentScreen == 2) {
				DisplayScore(totalScore);
				round_score = -1;
			}
		}
		if(totalScore >= WINNING_SCORE) {
			WinBuzzer();
			WinLight();
			_delay_ms(5000); // 5 seconds to bask in your victory before reset
			ResetGame();
		}
		else if (remainingTurns <= 0) {
			LoseBuzzer();
		}
	}
	return 0;
}













////////////////////
//    MY CODE     //
////////////////////

void SetupInterrupts(void)
{
	//Setup for Center Button Interrupt
	PCMSK0  |= (1<<PCINT3); // PE3 = right
	PCMSK0  |= (1<<PCINT2); // PE2 = left
	PCMSK1  |= (1<<PCINT14); // PB6 = up
	PCMSK1  |= (1<<PCINT15); // PB7 = down
	PCMSK1  |= (1<<PCINT12); // PB4 = push
	EIMSK   |= (1<<PCIE1);    //Enable interrupt for flag PCIF1
	EIMSK	|= (1<<PCIE0);
	
}


//This performs adjustments needed to undo actions of Butterfly boot loader
void BootLoaderFixes(void)
{
	//Boot loader Disables E2 and E3 digital input drivers, which are used for left and right
	//The following code re-enables them by clearing the appropriate disable bits
	DIDR1 &= ~((1<<AIN0D)|(1<<AIN1D));
}



// basically a print function for positive integers with a max of 3 digits
void DisplayScore(int value){
	char number;
	int digit;
	if (value >= 1000) {
		// first digit
		digit = value;
		digit = digit / 1000;
		number = (char)digit + 48;
		LCD_WriteDigit(number, 2);
		//second digit
		digit = value % 1000;
		digit = digit / 100; // digit should be a single integer (ie 580/100 = 5)
		number = (char)digit + 48; // get ascii value
		LCD_WriteDigit(number, 3);
		//third digit
		digit = value % 100;
		digit = digit/10; // digit should be a single integer (24/10 = 2);
		number = (char)digit + 48; // get the ascii value
		LCD_WriteDigit(number, 4);
		//fourth digit
		digit = value;
		digit = digit % 10; // get the ones place digit
		number = (char)digit + 48;
		LCD_WriteDigit(number, 5);
		LCD_WriteDigit('_', 0);
		LCD_WriteDigit('_', 1);
		return;
	}
	else if (value >= 100) { // 100 - 180
		//first digit
		digit = value;
		digit = digit / 100; // digit should be a single integer (ie 580/100 = 5)
		number = (char)digit + 48; // get ascii value
		LCD_WriteDigit(number, 3);
		//second digit
		digit = value % 100;
		digit = digit/10; // digit should be a single integer (24/10 = 2);
		number = (char)digit + 48; // get the ascii value
		LCD_WriteDigit(number, 4);
		//third digit
		digit = value;
		digit = digit % 10; // get the ones place digit
		number = (char)digit + 48;
		LCD_WriteDigit(number, 5);
		LCD_WriteDigit('_', 0);
		LCD_WriteDigit('_', 1);
		LCD_WriteDigit('_', 2);
		return;
	}
	
	else if (value >= 10) { // 10 - 99
		//first digit
		digit = value;
		digit = digit/10; // digit should be a single integer (24/10 = 2);
		number = (char)digit + 48; // get the ascii value
		LCD_WriteDigit(number, 4);
		//second digit
		digit = value;
		digit = digit % 10; // get the ones place digit
		number = (char)digit + 48;
		LCD_WriteDigit(number, 5);
		LCD_WriteDigit('_', 0);
		LCD_WriteDigit('_', 1);
		LCD_WriteDigit('_', 2);
		LCD_WriteDigit('_', 3);
		return;
	}
	
	else { //0 - 9
		digit = value;
		digit = digit % 10; // get the ones place digit
		number = (char)digit + 48;
		LCD_WriteDigit(number, 5);
		LCD_WriteDigit('_', 3);
		LCD_WriteDigit('_', 4);
		return;
	}
}


void DisplayTurnsRemaining(int turns) {
	char number;
	int digit;
	digit = turns % 10; // get 0 - 9 
	number = (char)(digit + 48); // get ascii value
	LCD_WriteDigit(number, 0);
}

void LoseBuzzer(void) {
	for (int i = 0 ; i < 5 ; i++) {
		for (int j = 0 ; j < 110 ; ++j) {
			PORTB ^= (1<<5);            //toggle PORTB5
			_delay_us(500);
			_delay_us(500);
			_delay_us(136);
		}
		_delay_ms(500);
	}
}

void WinBuzzer(void) {
	for (int i = 0 ; i < 2 ; i++) {
		for (int j = 0 ; j < 110 ; ++j) {
			PORTB ^= (1<<5);            //toggle PORTB5
			_delay_us(500);
			_delay_us(500);
			_delay_us(136);
		}
		_delay_ms(500);
	}
}

void WinLight(void) {
	for (int i = 0 ; i < 4 ; i++) { // need 4 because need to turn on, off, on, off
		PORTB ^= (1<<0);
		_delay_ms(500);
	}
}


void ResetGame(void){
	totalScore = 0;
	remainingTurns = 10;
	SpinSlot = 0;
	currentScreen = 1;
}






















////////////////////
// ISR DEFINITIONS//
////////////////////

ISR(PCINT1_vect)  {
	static uint8_t pinB6Prev=1; //for storing previous value of port to detect
	static uint8_t pinB7Prev=1;

	//toggle PORTB0 based on center button status being newly pressed, but not when it is released
	if(((PINB     & (1<<7))  == 0) && ((pinB7Prev & (1<<7))  != 0)) {
		SpinSlot = 1;	// start spinning the slot machine
		remainingTurns--; // remove a turn because you used one
	}
	
	else if(((PINB     & (1<<6))  == 0) && ((pinB6Prev & (1<<6))  != 0)) { //up: reset
		LCD_WriteDigit('R',0);
		LCD_WriteDigit('E',1);
		LCD_WriteDigit('S',2);
		LCD_WriteDigit('E',3);
		LCD_WriteDigit('T',4);
		LCD_WriteDigit('_',5);
		ResetGame();
		_delay_ms(500);
		LCD_WriteDigit('_',0);
		LCD_WriteDigit('_',1);
		LCD_WriteDigit('_',2);
		LCD_WriteDigit('_',3);
		LCD_WriteDigit('_',4);
		LCD_WriteDigit('_',5);
	}
	
	else if(((PINB     & (1<<4))  == 0) && ((pinB6Prev & (1<<4))  != 0)) { // pushbutton
		SpinSlot = 0; // stop Spinning the slot machine
		_delay_ms(500);		
	}
	
	
	else
	{
		//do nothing
	}
	pinB6Prev = PINB; //save button status
	pinB7Prev = PINB; //save button status


}

ISR(PCINT0_vect)  {		//remember this is called on pin change 0->1 and 1->0
	static uint8_t pinE2Prev=1;
	static uint8_t pinE3Prev=1;

	if(((PINE     & (1<<2))  == 0) && ((pinE3Prev & (1<<2))  != 0)) { // left: switch to slot screen
		LCD_WriteDigit('_',0);
		LCD_WriteDigit('_',1);
		LCD_WriteDigit('_',2);
		LCD_WriteDigit('_',3);
		LCD_WriteDigit('_',4);
		LCD_WriteDigit('_',5);
		currentScreen = 1;
		DisplayTurnsRemaining(remainingTurns);
		
	}

	else if(((PINE     & (1<<3))  == 0) && ((pinE3Prev & (1<<3))  != 0)) { //right: show total score screen
		currentScreen = 2;
		LCD_WriteDigit('_',0);
		LCD_WriteDigit('_',1);
		LCD_WriteDigit('_',2);
		LCD_WriteDigit('_',3);
		LCD_WriteDigit('_',4);
		LCD_WriteDigit('_',5);
		DisplayScore(totalScore);
	}
	else {
		//nothing
	}
	
	pinE2Prev=PINE;
	pinE3Prev=PINE;
}



