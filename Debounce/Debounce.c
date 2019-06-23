/*
 * toggle_debounced.c
 *
 * Created: 6/15/2019
 * Author : Miguel Osuna
 */ 

#define F_CPU 16000000UL
#define DEBOUNCE_TIME 1000 // Time in microseconds

/* Push-button Registers */
#define BUTTON_DDR DDRB
#define BUTTON_PORT PORTB
#define BUTTON_PIN PINB
#define BUTTON PB0

/* LED Registers */
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED PB5

#include <avr/io.h>
#include <util/delay.h>

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
	
	uint8_t buttonPressed = 0;
	
    while (1) 
    {
		/* If push-button is pressed, toggle LED */
		if(debounce())
		{
			if(!buttonPressed)
			{
				LED_PORT ^= (1 << LED);
				buttonPressed = 1;
			}
		}
		else
			buttonPressed = 0;
	}
	return 0;
}

