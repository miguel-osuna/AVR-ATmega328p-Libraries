/*
 * Interruptc.c
 *
 *  Created: 6/18/2019 
 *  Author: Miguel Osuna
 */ 

#include "Interrupt.h"

/* Enable External Interrupt
 * '0': INT0
 * '1': INT1 
 
 * '00': The low level of INT generates an interrupts request
 * '01': Any logical change on INT generates an interrupt request
 * '10': The falling edge of INT generates an interrupt request
 * '11': The rising edge of INT generates an interrupt request
 */
void enableINT(uint8_t interrupt, uint8_t mode)
{
	/* (EICRA): External Interrupt Control Register A */
	/* (ISC1n): Interrupt Sense Control [n = 1:0] */

	/* (EIMSK): External Interrupt Mask Register */
	EIMSK |= (1 << interrupt);
	
	if(interrupt)
		EICRA |= (mode << ISC10);
		
	if(!interrupt)
		EICRA |= (mode << ISC00);
		
	sei(); // Set Global Interrupt Enable Bit
}

/* Enable Pin Change Interrupt
 * Port '0': Port B
 * Port '1': Port C
 * Port '2': Port D */ 
void enablePCIE(uint8_t port, uint8_t interrupt)
{
	/* (PCICR): Pin Change Interrupt Control Register
	 * (PCIE): Pin Change Interrupt Enable */
	
	// Port Enable
	PCICR |= (1 << port);

	/* (PCMSK): Pin Change Mask Register
	 * (PCINTn): Pin Change Enable Mask [n = 7:0] */
	
	if(interrupt > 7)
		interrupt = 0;

	// Mask Register 0
	if(port == 0)
	{
		PCMSK0 |= (1 << interrupt);
	}
	
	// Mask Register 1
	if(port == 1)
	{
		PCMSK1 |= (1 << interrupt);
	}
	
	// Mask Register 2
	if(port == 2)
	{
		PCMSK2 |= (1 << interrupt);
	}
	sei();
}