/*
DS1302.h - Arduino library support for the DS1302 Trickle Charge Timekeeping Chip
Copyright (C)2010 Henning Karlsen. All right reserved

You can find the latest version of the library at 
http://www.henningkarlsen.com/electronics

This library has been made to easily interface and use the DS1302 RTC with
the Arduino.

If you make any modifications or improvements to the code, I would appreciate
that you share the code with me so that I might include it in the next release.
I can be contacted through http://www.henningkarlsen.com/electronics/contact.php

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef JYMCU3208_h
#define JYMCU3208_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define FORMAT_SHORT	1
#define FORMAT_LONG	2

#define FORMAT_LITTLEENDIAN	1
#define FORMAT_BIGENDIAN	2
#define FORMAT_MIDDLEENDIAN	3

#define MONDAY		1
#define TUESDAY		2
#define WEDNESDAY	3
#define THURSDAY	4
#define FRIDAY		5
#define SATURDAY	6
#define SUNDAY		7

#define TCR_D1R2K	165
#define TCR_D1R4K	166
#define TCR_D1R8K	167
#define TCR_D2R2K	169
#define TCR_D2R4K	170
#define TCR_D2R8K	171
#define TCR_OFF		92

#define REG_SEC		0
#define REG_MIN		1
#define REG_HOUR	2
#define REG_DATE	3
#define REG_MON		4
#define REG_DOW		5
#define REG_YEAR	6
#define REG_WP		7
#define REG_TCR		8

#define CE0 PORTB &= ~B00000100
#define CE1 PORTB |=  B00000100
#define SCLK0 PORTB &= ~B00000001
#define SCLK1 PORTB |=  B00000001
#define DATA0 PORTB &= ~B00000010
#define DATA1 PORTB |= B00000010
#define DATA_PIN_IN DDRB &= ~B00000010
#define DATA_PIN_OUT DDRB |= B00000010
#define DATA_READ (PINB & B00000010)==B00000010

#define HTport   PORTB
#define HTddr    DDRB
#define HTstrobe 3		// These are the AVR PortB numbers, not the Arduino equivelants
#define HTclk    4
#define HTdata   5

#define HTclk0    HTport&=~(1<<HTclk)
#define HTclk1    HTport|= (1<<HTclk)
#define HTstrobe0 HTport&=~(1<<HTstrobe)
#define HTstrobe1 HTport|= (1<<HTstrobe)
#define HTdata0   HTport&=~(1<<HTdata)
#define HTdata1   HTport|= (1<<HTdata)
#define HTpinsetup() do{  HTddr |=(1<<HTstrobe)|(1<<HTclk)|(1<<HTdata); HTport|=(1<<HTstrobe)|(1<<HTclk)|(1<<HTdata);  }while(0)

#define HTstartsys   0b100000000010 //start system oscillator
#define HTstopsys    0b100000000000 //stop sytem oscillator and LED duty    <default
#define HTsetclock   0b100000110000 //set clock to master with internal RC  <default
#define HTsetlayout  0b100001000000 //NMOS 32*8 // 0b100-0010-ab00-0  a:0-NMOS,1-PMOS; b:0-32*8,1-24*16   default:ab=10
#define HTledon      0b100000000110 //start LEDs
#define HTledoff     0b100000000100 //stop LEDs    <default
#define HTsetbright  0b100101000000 //set brightness b=0..15  add b<<1  //0b1001010xxxx0 xxxx:brightness 0..15=1/16..16/16 PWM
#define HTblinkon    0b100000010010 //Blinking on
#define HTblinkoff   0b100000010000 //Blinking off  <default
#define HTwrite      0b1010000000   // 101-aaaaaaa-dddd-dddd-dddd-dddd-dddd-... aaaaaaa:nibble adress 0..3F   (5F for 24*16)

// Key Definitions. 
// We will do all of this using C++ commands as it is more efficient than using the Arduino functions

#define key1 ((PIND&(1<<7))==0) // read the status of PD7 a bit quicker that using digitalRead(7);
#define key2 ((PIND&(1<<6))==0) // ditto for PD6  (Pin 6 on a normal Ardudino NG)
#define key3 ((PIND&(1<<5))==0) // ditto for PD5  (Pin 5 on a normal Ardudino NG)

// set as Keys as output and all high to enable the AVR's pullup resistors
#define keysetup() do{ DDRD&=0xff-(1<<7)-(1<<6)-(1<<5); PORTD|=(1<<7)+(1<<6)+(1<<5); }while(0)  //input, pull up

class HT1632C
{
public:
	HT1632C();
	void	send(uint16_t data, byte bits);
	void	brightness(byte b);
	void	sendscreen(void);
	void	sendCol(byte col, byte pattern);
	void	sendPixel(byte col, byte row, bool state);

private:
	void	_HTsetup();
	void	_HTcommand(uint16_t data);
	void	_HTsend(uint16_t data, byte bits);
	byte	_leds[32];

};

class Time
{
public:
	uint8_t		hour;
	uint8_t		min;
	uint8_t		sec;
	uint8_t		date;
	uint8_t		mon;
	uint16_t	year;
	uint8_t		dow;

	Time();
};

class DS1302_RAM
{
public:
	byte	cell[31];

	DS1302_RAM();
};

class DS1302
{
public:
	DS1302();
	Time	getTime();
	void	setTime(uint8_t hour, uint8_t min, uint8_t sec);
	void	setDate(uint8_t date, uint8_t mon, uint16_t year);
	void	setDOW(uint8_t dow);

	char	*getTimeStr(uint8_t format=FORMAT_LONG);
	char	*getDateStr(uint8_t slformat=FORMAT_LONG, uint8_t eformat=FORMAT_LITTLEENDIAN, char divider='.');
	char	*getDOWStr(uint8_t format=FORMAT_LONG);
	char	*getMonthStr(uint8_t format=FORMAT_LONG);

	void	halt(bool value);
	void	writeProtect(bool enable);
	void	setTCR(uint8_t value);

	void		writeBuffer(DS1302_RAM r);
	DS1302_RAM	readBuffer();
	void		poke(uint8_t addr, uint8_t value);
	uint8_t		peek(uint8_t addr);


private:
	//uint8_t _ce_pin;
	//uint8_t _data_pin;
	//uint8_t _sclk_pin;
	uint8_t _burstArray[8];


	uint8_t	_readByte();
	void	_writeByte(uint8_t value);
	uint8_t	_readRegister(uint8_t reg);
	void 	_writeRegister(uint8_t reg, uint8_t value);
	void	_burstRead();
	uint8_t	_decode(uint8_t value);
	uint8_t	_decodeH(uint8_t value);
	uint8_t	_decodeY(uint8_t value);
	uint8_t	_encode(uint8_t vaule);
	void _shiftOut(uint8_t val);    
};
#endif
