/*
 * toggle_debounced.c
 *
 * Created: 6/15/2019
 * Author : Miguel Osuna
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif 

#include <avr/io.h>
#include <util/delay.h>

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Debounce Definitions
 * //////////////////////////////////////////////////////////////////////////
 */ 
#define BUTTON_DDR DDRB
#define BUTTON_PORT PORTB
#define BUTTON_PIN PINB
#define BUTTON PB0
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED PB5
#define DEBOUNCE_TIME 1000 // Time in microseconds


/*
 * //////////////////////////////////////////////////////////////////////////
 *						Debounce Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 
uint8_t debounce()
{
	/* If push-button is pressed */
	if(!(BUTTON_PIN & (1 << BUTTON)))
	{
		_delay_us(DEBOUNCE_TIME);
		
		/* If push-button is still pressed */
		if(!(BUTTON_PIN & (1 << BUTTON)))
			return 1;
	}
	return 0;
}

int main(void)
{
    /* ----- Inits ----- */
	LED_DDR |= (1 << LED); // On-board LED as Output
	BUTTON_DDR &= ~(1 << PB1); // Push-button as Input
	BUTTON_PORT |= (1 << BUTTON); // Configure as Pull-up Resistor
	
	uint8_t button_pressed = 0;
	
    while (1) 
    {
		/* If push-button is pressed, toggle LED */
		if(debounce())
		{
			if(!button_pressed)
			{
				LED_PORT ^= (1 << LED);
				button_pressed = 1;
			}
		}
		else
			button_pressed = 0;
	}
	return 0;
}

