#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"

#define BLA_DDR DDRB
#define BLA_PIN PINB
#define BLA_PORT PORTB
#define BLA 2

#define SID_DDR DDRB
#define SID_PIN PINB
#define SID_PORT PORTB
#define SID 1

#define SCLK_DDR DDRB
#define SCLK_PIN PINB
#define SCLK_PORT PORTB
#define SCLK 0

#define A0_DDR DDRD
#define A0_PIN PIND
#define A0_PORT PORTD
#define A0 7

#define RST_DDR DDRD
#define RST_PIN PIND
#define RST_PORT PORTD
#define RST 6

#define CS_DDR DDRD
#define CS_PIN PIND
#define CS_PORT PORTD
#define CS 5

#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED 2

uint8_t is_reversed = 0;

int pagemap[] = { 3, 2, 1, 0, 7, 6, 5, 4 };

uint8_t buffer[128*64/8] = {
	0x7F,0x7F,0x7F,0x7F,0x71,0x7F,0x71,0x7F,0x7F,0x7F,0x7F,0x7F,0x7E,0x7F,0x7A,0x7B,
	0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x71,0x7F,0x7F,0x71,0x7F,0x7F,0x7F,
	0x00,0x00,0x60,0x60,0x60,0x78,0x7F,0x7F,0x7F,0x7F,0x7F,0x60,0x60,0x60,0x60,0x60,
	0x60,0x70,0x70,0x78,0x78,0x3E,0x3F,0x1F,0x1F,0x0F,0x07,0x01,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0xFF,0xFF,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF1,0xF3,0xF7,0xFF,
	0xFF,0xFF,0xF7,0xF1,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xF0,0xFF,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x83,0xFF,0xFF,0xFF,0xFF,0xFF,0x3C,0x00,0x01,0x01,
	0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,
	0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x00,0x00,
	0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,

	0xE0,0xFF,0x03,0x00,0x01,0x03,0x07,0x0F,0x1E,0x3E,0x7F,0xFF,0xFF,0xFF,0x9F,0x0F,
	0x0F,0x8F,0xFF,0xFF,0xFF,0x7F,0x3E,0x1E,0x0F,0x07,0x03,0x01,0x00,0x01,0x3F,0xFC,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x1C,0x1C,0x1C,0x1C,0x1C,
	0x1C,0x1C,0x1C,0x3C,0x78,0xF8,0xF8,0xF8,0xF1,0xEF,0xDF,0xFF,0x7F,0xFF,0xE1,0xC1,
	0x81,0x81,0x81,0x81,0x81,0xC1,0xE1,0xF9,0xFF,0xFF,0x7F,0x1F,0x87,0x80,0x80,0xC0,
	0xFF,0xFF,0xFF,0xFF,0xFF,0x78,0xF0,0xE0,0xC0,0xC0,0xC0,0xC0,0xC0,0xE0,0xFF,0xFF,
	0xFF,0xFF,0x3F,0x07,0x00,0x00,0x80,0x80,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0x3C,0x78,
	0xF0,0xE0,0xC0,0xC0,0xC0,0xC0,0xE0,0xF0,0xFF,0xFF,0xFF,0x7F,0x1F,0x00,0x00,0x00,

	0x00,0x80,0xE0,0xF8,0xFC,0xFF,0xFF,0x3F,0x1F,0x1E,0x3C,0xF8,0xF0,0xE0,0xC0,0x80,
	0x00,0xC0,0xE0,0xF0,0xF8,0x3C,0x1E,0x1F,0x3F,0xFF,0xFF,0xFC,0xF8,0xE0,0x80,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF,0xFF,0xFF,0xBF,0x81,0x80,
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,
	0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x30,0x18,0x0C,0x0C,0x06,0x06,
	0x06,0x0C,0x0C,0x18,0x18,0x30,0x60,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x06,0x0E,0x0E,0x3E,0xFE,0xFE,0xFE,0xFE,0xFE,0x7E,0x0E,0x0E,0x06,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xF0,0xF8,0xFC,0xFC,0x7E,
	0x3E,0x1E,0x0E,0x0E,0x0E,0x06,0x0E,0x0E,0x0E,0x1C,0x38,0x78,0xE6,0x06,0x0E,0x1E,
	0xFE,0xFE,0xFE,0xFE,0xFE,0x0E,0x0E,0x06,0x00,0x00,0x00,0x06,0x06,0x0E,0xFE,0xFE,
	0xFE,0xFE,0xFE,0xFE,0x0E,0x0E,0x06,0x06,0x0E,0xFE,0xFE,0xFE,0xFE,0xFE,0x7E,0x0E,
	0x0E,0x06,0x00,0x00,0x00,0x06,0x0E,0x0E,0xFE,0xFE,0xFE,0xFE,0xFE,0x1E,0x0E,0x06,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	0x00,0x00,0xFF,0xFF,0xCE,0xCE,0xCE,0xCE,0x00,0x1F,0x1F,0x1F,0x38,0x38,0x1F,0x1F,
	0x1C,0x38,0x3C,0x1F,0x0F,0x00,0xFF,0xFF,0x1C,0x38,0x38,0x1F,0x1F,0x07,0x07,0x1F,
	0x1F,0x3B,0x3B,0x1F,0x1F,0x07,0x07,0x1F,0x1F,0x38,0x38,0xFF,0xFF,0xFF,0x00,0x0F,
	0x1F,0x38,0x38,0x1C,0xFF,0xFF,0x00,0x0F,0x1F,0x1F,0x3B,0x3B,0x1F,0x1F,0x07,0x0F,
	0x1F,0x1F,0x38,0x38,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x78,0xFC,0xFC,0xCE,0xCF,
	0xC7,0xC3,0x18,0x1F,0x1F,0x03,0x03,0x1F,0x1F,0x18,0x1E,0x1F,0x3F,0x3B,0x1B,0x01,
	0x18,0xFF,0xFF,0x18,0x18,0x00,0x0F,0x1F,0x3B,0x3B,0x3B,0x1F,0x0F,0x00,0x1F,0x1F,
	0x1C,0x38,0x3C,0x1F,0x1F,0x18,0x38,0x3F,0x1F,0x07,0x00,0x1F,0x3F,0x3B,0x3B,0x19,

	0x00,0x00,0xF0,0xF0,0x70,0x70,0x70,0x70,0x00,0xF0,0xF0,0xF0,0x00,0x00,0xF0,0xF0,
	0x00,0x00,0x00,0xF0,0xF0,0x00,0xF0,0xF0,0xF0,0x70,0x70,0xF0,0xE0,0x80,0xC0,0xE0,
	0xF0,0x70,0x30,0x70,0x70,0x00,0xC0,0xE0,0xF0,0x70,0x70,0xE0,0xF0,0xF0,0x00,0xE0,
	0xF0,0x70,0x70,0x70,0xF0,0xF0,0x00,0xC0,0xE0,0xF0,0x70,0x30,0x70,0x70,0x00,0xC0,
	0xE0,0xF0,0x70,0x70,0xF0,0xF0,0xF0,0x00,0x00,0x00,0x00,0x60,0x70,0x70,0x30,0x70,
	0xF0,0xE0,0x00,0x00,0xEE,0xFE,0xFC,0xE0,0x00,0x00,0x70,0x70,0x30,0xF0,0xF0,0xE0,
	0x00,0xE0,0xF0,0x70,0x70,0x00,0xE0,0xF0,0x70,0x30,0x30,0x70,0x60,0x00,0xF0,0xF0,
	0x00,0x00,0x00,0xF0,0xF0,0x00,0x00,0xF0,0xF0,0xF0,0x60,0x70,0x30,0xB0,0xF0,0xE0
};

void clear_screen(void) {
  uint8_t p, c;
  
  for(p = 0; p < 8; p++) {
    
    lcd_command(CMD_SET_PAGE | p);
    for(c = 0; c < 129; c++) {
      lcd_command(CMD_SET_COLUMN_LOWER | (c & 0xf));
      lcd_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0xf));
      lcd_data(0x0);
    }     
  }
}


void lcd_init(void) {
  // set pin directions
  SID_DDR |= _BV(SID);
  SCLK_DDR |= _BV(SCLK);
  A0_DDR |= _BV(A0);
  RST_DDR |= _BV(RST);
  CS_DDR |= _BV(CS);
  
  // toggle RST low to reset; CS low so it'll listen to us
  CS_PORT &= ~_BV(CS);
  RST_PORT &= ~_BV(RST);
  _delay_ms(500);
  RST_PORT |= _BV(RST);

  // LCD bias select
  lcd_command(CMD_SET_BIAS_7);
  // ADC select
  lcd_command(CMD_SET_ADC_NORMAL);
  // SHL select
  lcd_command(CMD_SET_COM_NORMAL);
  // Initial display line
  lcd_command(CMD_SET_DISP_START_LINE);

  // turn on voltage converter (VC=1, VR=0, VF=0)
  lcd_command(CMD_SET_POWER_CONTROL | 0x4);
  // wait for 50% rising
  _delay_ms(50);

  // turn on voltage regulator (VC=1, VR=1, VF=0)
  lcd_command(CMD_SET_POWER_CONTROL | 0x6);
  // wait >=50ms
  _delay_ms(50);

  // turn on voltage follower (VC=1, VR=1, VF=1)
  lcd_command(CMD_SET_POWER_CONTROL | 0x7);
  // wait
  _delay_ms(10);

  // set lcd operating voltage (regulator resistor, ref voltage resistor)
  lcd_command(CMD_SET_RESISTOR_RATIO | 0x6);

}

inline void spiwrite(uint8_t c) {
  int8_t i;
  for (i=7; i>=0; i--) {
    SCLK_PORT &= ~_BV(SCLK);
    if (c & _BV(i))
      SID_PORT |= _BV(SID);
    else
      SID_PORT &= ~_BV(SID);
    SCLK_PORT |= _BV(SCLK);
  }

 
}
void lcd_command(uint8_t c) {
  A0_PORT &= ~_BV(A0);

  spiwrite(c);
}

void lcd_data(uint8_t c) {
  A0_PORT |= _BV(A0);

  spiwrite(c);
}
void lcd_set_brightness(uint8_t val) {
    lcd_command(CMD_SET_VOLUME_FIRST);
    lcd_command(CMD_SET_VOLUME_SECOND | (val & 0x3f));
}


void write_buffer(uint8_t *buffer) {
  uint8_t c, p;

  for(p = 0; p < 8; p++) {
    
    lcd_command(CMD_SET_PAGE | pagemap[p]);
    lcd_command(CMD_SET_COLUMN_LOWER | (0x0 & 0xf));
    lcd_command(CMD_SET_COLUMN_UPPER | ((0x0 >> 4) & 0xf));
    lcd_command(CMD_RMW);
    lcd_data(0xff);
    
    
    for(c = 0; c < 128; c++) {
      lcd_data(buffer[(128*p)+c]);
    }
  }
}

