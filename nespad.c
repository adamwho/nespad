/*
 * 		NESPAD - I2C Slave for driving two NES Gamepads
 * 		This Code is free - in every sense of meaning..
 *
 * 		Author: Adam Who (dafuq)
 * 		Website: adamwho.wordpress.com
 *
 * 		greetings: - hackaday.com
 * 				   - dangerousprototypes.com
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

#include "nespad.h"
#include "bithacks.h"
#include "usiTwiSlave.h"


#define NO_ZOMBIE_APOCALYPSE_YET 0

void nespad_poll_gamepad(nespad_t* gamepad);

void i2cWriteToRegister(uint8_t reg, uint8_t value);
uint8_t i2cReadFromRegister(uint8_t reg);


void setup()
{
	// Init USI TWI Slave and enable interrupts
	usiTwiSlaveInit(NESPAD_SLAVE_ADDRESS, i2cReadFromRegister, i2cWriteToRegister);
	sei();

	// clock / latch pair for gamepad 1
	bit_set(DDRD, BIT(NES_CLOCK_1));
	bit_set(DDRD, BIT(NES_LATCH_1));

	// clock / latch pair for gamepad 2
	bit_set(DDRB, BIT(NES_CLOCK_2));
	bit_set(DDRB, BIT(NES_LATCH_2));

	// shared data pin for all gamepads
	bit_clr(DDRD, BIT(NES_DATA));
}

int main()
{
	setup();

	while(NO_ZOMBIE_APOCALYPSE_YET)
	{
		for(int i = 0; i < MAX_GAMEPADS;i++){
			nespad_poll_gamepad(&nes_gamepads[i]);
		}
	}

	return 0;
}

void nespad_poll_gamepad(nespad_t* gamepad)
{
	uint8_t local_buff = 0x00;
	// Toggle Latch - which presents the first bit
	bit_set(*gamepad->port, BIT(gamepad->latch_pin));
	_delay_us(PULSE_TIME);
	bit_clr(*gamepad->port, BIT(gamepad->latch_pin));
	_delay_us(PULSE_TIME);

	// Read first bit
	local_buff = bit_get(PIND,BIT(gamepad->data_pin));

	// Now get the next 7 bits with the clock
	  for (int i = 0 ; i < 7 ; ++i)
	  {
		bit_set(*gamepad->port, BIT(gamepad->clock_pin));
		_delay_us(PULSE_TIME);
		bit_clr(*gamepad->port, BIT(gamepad->clock_pin));
	    _delay_us(PULSE_TIME);
	    local_buff = (local_buff << 1) | bit_get(PIND,BIT(gamepad->data_pin));
	  }

	  gamepad->buffer = local_buff;

	  return;
}

// A callback triggered when the i2c master attempts to read from a register.
uint8_t i2cReadFromRegister(uint8_t reg)
{
        if(reg < MAX_GAMEPADS){
        	return nes_gamepads[reg].buffer;
        }else{
        	return 0xFF;
        }
}

// A callback triggered when the i2c master attempts to write to a register.
void i2cWriteToRegister(uint8_t reg, uint8_t value)
{

}

