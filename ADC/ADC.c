/*
 * adc.c
 *
 * Created: 6/16/2019
 * Author: Miguel Osuna
 */ 

#include "adc.h"

/*
 * ///////////////////////////////////////////////////
 *					ADC Functions
 * ///////////////////////////////////////////////////
 */ 

/* 
 * Sets Reference
 * '00': AREF, Internal Vref turned off
 * '01': AVcc with external capacitor at AREF pin
 * '00': Reserved 
 * '11': Internal 1.1V Voltage Reference with external capacitor at AREF pin 
 */
static inline void set_reference(uint8_t ref)
{
	// (ADMUX): ADC Multiplexer Selection Register
	// (REFSn): Reference Selection [n = 1:0] 

	if(ref == 2 || ref > 3)
		ref = 0;
	ADMUX |= (ref << REFS0);
}

/* 
 * Sets Adjust Result 
 * '1': Left Adjust Result
 * '0': Right Adjust Result, this is by default 
 */
static inline void set_adjust(uint8_t adjust)
{
	// (ADMUX): ADC Multiplexer Selection Register
	// (ADLAR): ADC Left Adjust Register
	
	// Left Adjust Result
	if(adjust)
		ADMUX |= (1 << ADLAR);
		
	// Right Adjust Result 
	else
		ADMUX &= ~(1 << ADLAR);
}

/* 
 * Sets Channel for Input 
 * '0': ADC0
 * '1': ADC1
 * '2': ADC2
 * '3': ADC3
 * '4': ADC4
 * '5': ADC5
 * '6': ADC6
 * '7': ADC7 
 */
static inline void set_channel(uint8_t channel)
{
	// (ADMUX): ADC Multiplexer Selection Register
	// (MUXn): Analog Channel Selection [n = 3:0]
	
	if(channel > 7)
		channel = 0;

	// Clears ADMUX MUXn bits and sets new Channel
	ADMUX = (0xf0 & ADMUX) | channel;
}

static inline void set_prescaler(uint8_t prescaler)
{
	// (ADCSRA): ADC Control and Status Register A
	// (ADPSn): ADC Prescaler Select [n = 2:0]
	
	if(prescaler != 2 || prescaler != 4 || prescaler != 8 || prescaler != 16 || prescaler != 32 || prescaler != 64 || prescaler != 128)
		prescaler = 128;

	ADCSRA |= (prescaler << ADPS0);
}

/* 
 * Initialize ADC
 * '1': Single Mode Conversion
 * '0': Running Mode Conversion 
 */
void init_adc(uint8_t prescaler, uint8_t adjust, uint8_t ref, uint8_t mode)
{
	// (ADATE): ADC Auto Trigger Enable
	// (ADsingle_conversion_adc): ADC Control and Status Register A

	// Enable ADC
	ADCSRA |= (1 << ADEN);
	
	set_prescaler(prescaler);
	set_reference(ref);
	set_adjust(adjust);
	
	// Free Running Mode Conversion
	if(!mode)
	{
		// Auto Trigger Enable
		ADCSRA |= (1 << ADATE);
		
		// Free running mode
		ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADPS0));
	}
}

/* ADC Single Conversion Mode */
static inline uint16_t single_conversion_adc()
{
	// (ADCSRA): ADC Control and Status Register A 
	// (ADSC): ADC Start Conversion)
	
	uint16_t adc_reading;			// ADC reading variable
	ADCSRA |= (1 << ADSC);			// Start Conversion
	while(!(ADCSRA & (1 << ADSC))); // Wait until done
	adc_reading = ADC;				// Read ADC in
	
	// Left Adjust Result. Therefore, only High Register Needed
	if(ADMUX & (1 << ADLAR))
		adc_reading = ADCH;
		
	// Right Adjust Result. Full Register Needed
	else
		adc_reading = ADC;
		
	return adc_reading;
}

/* ADC Free Running Mode */
static inline uint16_t free_running_adc()
{
	uint16_t adc_reading;
	// (ADCSRA): ADC Control and Status Register A
	// (ADSC): ADC Start Conversion) */
	
	ADCSRA |= (1 << ADSC);  // Start Conversion
	
	// Left Adjust Result. Therefore, only High Register Needed
	if(ADMUX & (1 << ADLAR))
		adc_reading = ADCH;
	
	// Right Adjust Result. Full Register Needed
	else
		adc_reading = ADC;
				
	return adc_reading;
}

/* Read ADC */
uint16_t read_adc(uint8_t channel)
{
	uint16_t adc_reading;
	// Set Channel for the Reading
	set_channel(channel);
	
	// Free Running Mode Conversion
	if(!(ADCSRB & ((1 << ADTS2) | (1 << ADTS1) | (1 << ADPS0)))) // Check for free running mode
		adc_reading = free_running_adc();
		
	// Single Mode Conversion
	else
		adc_reading = single_conversion_adc();

	return reading;
}			