#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include "lcd.h"

#define FREQ 16000000
#define BAUD 9600
#define HIGH 1
#define LOW 0
#define BUFFER 1024 
#define BLACK 0x000001

int main(void)
{
	
	// DDRD |= 0x1C;		
	// PORTD &= ~0x1C;		
	// PORTD |= 0x00;   
    lcd_init();
	lcd_command(CMD_DISPLAY_ON);
 	lcd_set_brightness(0x18);
	write_buffer(buffer);
	_delay_ms(1500);
	
	 
	while (1) 
    {

    }
}

