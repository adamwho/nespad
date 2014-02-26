/*
 * 		NESPAD - I2C Slave for driving two NES Gamepads
 * 		This Code is free - in every sence of meaning..
 *
 * 		Author: Adam Who (dafuq)
 * 		Website: adamwho.wordpress.com
 *
 * 		greetings: - hackaday.com
 * 				   - dangerousprototypes.com
 */

#ifndef NESPAD_H
#define NESPAD_H

#include <avr/io.h>

#define NESPAD_SLAVE_ADDRESS 0x34
#define NES_HI 1;
#define NES_LO 0;

// Pin defines
#define NES_DATA	PIND4
#define NES_CLOCK_1 PD2
#define NES_LATCH_1 PD3
#define NES_CLOCK_2 PB3
#define NES_LATCH_2 PB2


#define	PULSE_TIME	25
#define MAX_GAMEPADS 2

typedef struct {
	volatile uint8_t* port;
	uint8_t clock_pin;
	uint8_t latch_pin;
	uint8_t data_pin;
	uint8_t buffer;
}nespad_t;

nespad_t nes_gamepads[MAX_GAMEPADS] = {
								{&PORTD, NES_CLOCK_1, NES_LATCH_1, NES_DATA, 0x00},
								{&PORTB, NES_CLOCK_2, NES_LATCH_2, NES_DATA, 0x00}
							  };

#endif

