/*
 * TIMER.c
 *
 * Created: 6/20/2019
 * Author: Miguel Osuna
 */ 

#include "TIMER.h"

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Timer/Counter 1 Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 

/* Timer/Counter 1 Prescaler
 * '0': No clock source (Timer/Counter stopped)
 * '1': clk/1 (No prescaling)
 * '2': clk/8 (From prescaler)
 * '3': clk/64 (From prescaler)
 * '4': clk/256 (From prescaler)
 * '5': clk/1024 (From prescaler) */
void setTimer1Prescaler(uint16_t prescaler)
{
	/* Start the Timer, by Setting the Prescaler*/
	/* (TCCR1B): TC1 Control Register 1 B */
	/* (CS1n): Clock Select 1 [n = 0:2] */
	TCCR1B |= (prescaler << CS10);	
}

/*  Timer 1 Waveform Generation Mode 
 * '0': Normal
 * '1': CTC (Clear Timer on Compare Match) -> TOP: OCR1A
 * '2': CTC (Clear Timer on Compare Match) -> TOP: ICR1 */
void setTimer1Waveform(uint8_t waveform)
{
	switch(waveform)
	{
		/* (TCCR1A): TC1 Control Register A */
		/* (TCCR1B): TC1 Control Register B */
		/* (WGMnn): Waveform Generation Mode */
		/* Normal Mode */
		case 0:
			TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
			TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
			break;	
			
		/* CTC - TOP: OCR1A */
		case 1:
			TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
			TCCR1B |= (1 << WGM12);
			TCCR1B &= ~(1 << WGM13);
			break;	
			
		/* CTC - TOP: ICR1 */
		case 2:
			TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
			TCCR1B |= ((1 << WGM13) | (1 << WGM12));
			break;	
			
		/* Normal Mode by Default */
		default:
			TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
			TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
	}
}

/* Timer 1 Compare Output Mode for Channel (non-PWM) 
 * '0': Normal port operation, OC1A/OC1B disconnected
 * '1': Toggle OC1A/OC1B on Compare Match
 * '2': Clear OC1A/OC1B on Compare Match
 * '3': Set OC1A/OC2B on Compare Match */
void setTimer1Compare(uint8_t mode)
{
	/* (TCCR1A): TC1 Control Register A */
	/* (COM1xn): Compare Output Mode for Channel */
	TCCR1A |= (mode << COM1A0);
}

/* Set Timer1 Counter for Specified Time */
void setTimer1Ticks(uint16_t prescaler, uint16_t milliseconds)
{
	/* Set the duration of the timer */
	/* (OCR1A): TC1 Output Compare Register A */
	if(prescaler == 2)
		prescaler = 8;
	else if(prescaler == 3)
		prescaler = 64;
	else if(prescaler == 4)
		prescaler = 256;
	else if(prescaler == 5)
		prescaler = 1024;
		
	uint16_t ticks; // Up to 65535
	ticks = (uint16_t) (((F_CPU) / (prescaler) * (milliseconds) / (1000)) - 1);
	OCR1A |= ticks;
}

/* Timer 1 Interrupt Enable
 * Sets the Interrupt corresponded to the Waveform Configuration */
void setTimer1Interrupt(uint8_t interruptWaveForm)
{
	/* Waveform Mode 
	* '0': Normal
	* '1': CTC (Clear Timer on Compare Match) -> TOP: OCR1A
	* '2': CTC (Clear Timer on Compare Match) -> TOP: ICR1 */ 	
	switch(interruptWaveForm)
	{
		/* (TIMSK1): Timer/Counter 1 Interrupt Mask Register */
		/* (TOIE1): Overflow Interrupt Enable 1 */
		case 0: 
			TIMSK1 |= (1 << TOIE1);
			break;
			
		/* (OCIE1A): Output Compare A Match Interrupt Enable 1 */		
		case 1:
			TIMSK1 |= (1 << OCIE1A);
			break;
			
		/* (ICIE): Input Capture Interrupt Enable 1 */		
		case 2:
			TIMSK1 |= (1 << ICIE1);
			break;
	}
	sei();
}

/* Initialize Timer 1 */
void initTimer1(uint8_t waveformMode, uint16_t milliseconds, uint8_t compareMode, uint16_t prescaler, uint8_t interrupt, void (*f)())
{
	/* Set Timer 1 Waveform Mode */
	setTimer1Waveform(waveformMode);
	
	if(waveformMode) // If Waveform Mode is CTC, set Timer 1 Ticks
		/* Set Timer 1 Ticks */
		setTimer1Ticks(prescaler, milliseconds);	
	
	/* Set Timer 1 Compare Output Mode */
	setTimer1Compare(compareMode);

	/* Configure Interrupt Mode if Enabled */
	if(interrupt)
	{
		setTimer1Interrupt(waveformMode); // The interrupt has to match the Waveform Mode
		timer1FuncPtr = f;				  // Copy f to timer1FuncPtr
	}
		
	/* Set Prescaler */
	setTimer1Prescaler(prescaler);
}

/* Sets TC1 to 0 */
void clearTimer1()
{
	/* (TCNT1): TC1 Counter Value */
	TCNT1 = 0;	
}

/* Resets TC1 Counter Value */
void resetTimer1()
{
	/* Clears the CTC Flag (Writing a logic one to the set flag clears it) */
	/* The timer overflow is also restarted */
	TIFR1 |= (1 << TOV1);
}

/* Returns TC1 Counter Value */
uint16_t valueTimer1()
{
	/* (TCNT1): TC1 Counter Value */
	return TCNT1;
}

/* Disable Timer/Counter 1 */
void stopTimer1()
{
	/* Stop Timer/Counter by Setting 'No clock source' for Waveform */
	/* (TCCR1B): TC1 Control Register 1 B */
	/* (CS1n): Clock Select 1 [n = 0:2] */
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

/* Watch for an Overflow Event - No interruptions enabled */
uint8_t checkTimer1Overflow()
{
	/* (TIFR1): TC1 Interrupt Flag Register */
	/* (TOV1): Timer/Counter 1, Overflow Flag */
	if((TIFR1 & (1 << TOV1)))	// Wait until occurs the overflow
		return 1;	
	return 0;
}

/* Timer 1 Interrupts, enable the desired Interrupt on the TIMER.h file */
#ifdef TIMER1_INTERRUPT_OCIEA
ISR(TIMER1_COMPA_vect)
{
	timer1FuncPtr();
}
#endif

#ifdef TIMER1_INTERRUPT_TOEI
ISR(TIMER1_OVF_vect)
{
	timer1FuncPtr();
}
#endif

#ifdef TIMER1_INTERRUPT_ICIE
ISR(TIMER1_CAPT_vect)
{
	timer1FuncPtr();
}
#endif

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Timer/Counter 0 Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 

/* Timer/Counter 0 Prescaler
 * '0': No clock source (Timer/Counter stopped)
 * '1': clk/1 (No prescaling)
 * '2': clk/8 (From prescaler)
 * '3': clk/64 (From prescaler)
 * '4': clk/256 (From prescaler)
 * '5': clk/1024 (From prescaler) */
void setTimer0Prescaler(uint16_t prescaler)
{
	/* Start the Timer, by Setting the Prescaler*/
	/* (TCCR0B): TC0 Control Register 0 B */
	/* (CS0n): Clock Select 0 [n = 0:2] */
	TCCR0B |= (prescaler << CS00);	
}

/* Timer 0 Waveform Generation Mode 
 * '0': Normal
 * '1': CTC (Clear Timer on Compare Match) -> TOP: OCR0A */
void setTimer0Waveform(uint8_t waveform)
{
	switch(waveform)
	{
		/* (TCCR0A): TC0 Control Register A */
		/* (TCCR0B): TC0 Control Register B */
		/* (WGM0n): Waveform Generation Mode */
		/* Normal Mode */
		case 0:
			TCCR1A &= ~((1 << WGM01) | (1 << WGM00));
			TCCR1B &= ~((1 << WGM02));
			break;	
			
		/* CTC - TOP: OCR0A */
		case 1:
			TCCR1A &= ~((1 << WGM00));
			TCCR1A |= (1 << WGM01);
			TCCR1B &= ~(1 << WGM02);
			break;	
			
		/* Normal Mode by Default */
		default:
			TCCR1A &= ~((1 << WGM01) | (1 << WGM00));
			TCCR1B &= ~((1 << WGM02));
	}
}

/* Timer 0 Compare Output Mode for Channel (non-PWM) 
 * '0': Normal port operation, OC0A disconnected
 * '1': Toggle OC0A on Compare Match
 * '2': Clear OC0A on Compare Match
 * '3': Set OC0A on Compare Match */
void setTimer0Compare(uint8_t mode)
{
	/* (TCCR0A): TC0 Control Register A */
	/* (COM0xn): Compare Output Mode for Channel */
	TCCR0A |= (mode << COM0A0);
}

/* Set Timer0 Counter for Specified Time */
void setTimer0Ticks(uint16_t prescaler, uint8_t milliseconds)
{
	/* Set the duration of the timer */
	/* (OCR0A): TC0 Output Compare Register A */
	if(prescaler == 2)
		prescaler = 8;
	else if(prescaler == 3)
		prescaler = 64;
	else if(prescaler == 4)
		prescaler = 256;
	else if(prescaler == 5)
		prescaler = 1024;
		
	uint8_t ticks;
	ticks = (uint8_t) (((F_CPU) / (prescaler) * (milliseconds) / (1000)) - 1);
	OCR0A |= ticks;
}

/* Timer 0 Interrupt Enable
 * Sets the Interrupt corresponded to the Waveform Configuration */
void setTimer0Interrupt(uint8_t interruptWaveForm)
{
	/* Timer 0 Waveform Generation Mode 
	 * '0': Normal
	 * '1': CTC (Clear Timer on Compare Match) -> TOP: OCR0A */
	switch(interruptWaveForm)
	{
		/* (TIMSK0): Timer/Counter 0 Interrupt Mask Register */
		/* (TOIE0): Overflow Interrupt Enable */
		case 0: 
			TIMSK0 |= (1 << TOIE0);
			break;
			
		/* (OCIE0A): Output Compare A Match Interrupt Enable */		
		case 1:
			TIMSK0 |= (1 << OCIE0A);
			break;
	}
	sei();
}

/* Initialize Timer 0 */
void initTimer0(uint8_t waveformMode, uint8_t milliseconds, uint8_t compareMode, uint16_t prescaler, uint8_t interrupt, void (*f)())
{
	/* Set Timer 0 Waveform Mode */
	setTimer0Waveform(waveformMode);
	
	if(waveformMode)	// If Waveform Mode is CTC, set Timer 0 Ticks
		/* Set Timer 0 Ticks */
		setTimer0Ticks(prescaler, milliseconds);	
	
	/* Set Timer 0 Compare Output Mode */
	setTimer0Compare(compareMode);

	/* Configure Interrupt Mode if Enabled */
	if(interrupt)
	{
		setTimer0Interrupt(waveformMode);	// The interrupt has to match the Waveform Mode
		timer0FuncPtr = f;	// Copy f to timer1FuncPtr
	}
		
	/* Set Prescaler */
	setTimer0Prescaler(prescaler);
}

/* Set TC0 to 0 */
void clearTimer0()
{
	/* (TCNT0): TC0 Counter Value */
	TCNT0 = 0;	
}

/* Resets TC0 Counter Value */
void resetTimer0()
{
	/* Clears the CTC Flag (Writing a logic one to the set flag clears it) */
	/* The timer overflow is also restarted */
	TIFR0 |= (1 << TOV0);
}

/* Returns TC0 Counter Value */
uint8_t valueTimer0()
{
	/* (TCNT0): TC0 Counter Value */
	return TCNT0;
}

/* Disable Timer/Counter 0 */
void stopTimer0()
{
	/* Stop Timer/Counter by Setting 'No clock source' for Waveform */
	/* (TCCR0B): TC0 Control Register 0 B */
	/* (CS0n): Clock Select 0 [n = 0:2] */
	TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}

/* Watch for an Overflow Event - No interruptions enabled */
uint8_t checkTimer0Overflow()
{
	/* (TIFR0): TC0 Interrupt Flag Register */
	/* (TOV0): Timer/Counter 0, Overflow Flag */
	// Wait until occurs the overflow
	if((TIFR0 & (1 << TOV0)))
		return 1;	
	return 0;
}

/* Timer 0 Interrupts, enable the desired Interrupt on the TIMER.h file */
#ifdef TIMER0_INTERRUPT_OCIEA
ISR(TIMER0_COMPA_vect)
{
	timer0FuncPtr();
}
#endif

#ifdef TIMER0_INTERRUPT_TOEI
ISR(TIMER0_OVF_vect)
{
	timer0FuncPtr();
}
#endif

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Timer/Counter 2 Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 

/* 
 * Timer/Counter 2 Prescaler
 * '0': No clock source (Timer/Counter stopped)
 * '1': clk/1 (No prescaling)
 * '2': clk/8 (From prescaler)
 * '3': clk/64 (From prescaler)
 * '4': clk/256 (From prescaler)
 * '5': clk/1024 (From prescaler)
 */
void setTimer2Prescaler(uint16_t prescaler)
{
	/* Start the Timer, by Setting the Prescaler*/
	/* (TCCR2B): TC0 Control Register 2 B */
	/* (CS2n): Clock Select 2 [n = 0:2] */
	TCCR2B |= (prescaler << CS20);	
}

/* 
 * Timer 2 Waveform Generation Mode 
 * '0': Normal
 * '1': CTC (Clear Timer on Compare Match) -> TOP: OCR2A
 */
void setTimer2Waveform(uint8_t waveform)
{
	switch(waveform)
	{
		/* (TCCR2A): TC2 Control Register A */
		/* (TCCR2B): TC2 Control Register B */
		/* (WGM2n): Waveform Generation Mode */
		/* Normal Mode */
		case 0:
			TCCR2A &= ~((1 << WGM21) | (1 << WGM20));
			TCCR2B &= ~((1 << WGM22));
			break;	
			
		/* CTC - TOP: OCR2A */
		case 1:
			TCCR2A &= ~((1 << WGM20));
			TCCR2A |= (1 << WGM21);
			TCCR2B &= ~(1 << WGM22);
			break;	
			
		/* Normal Mode by Default */
		default:
			TCCR2A &= ~((1 << WGM21) | (1 << WGM20));
			TCCR2B &= ~((1 << WGM22));
	}
}

/* Timer 2 Compare Output Mode for Channel (non-PWM) 
 * '0': Normal port operation, OC2A disconnected
 * '1': Toggle OC2A on Compare Match
 * '2': Clear OC2A on Compare Match
 * '3': Set OC2A on Compare Match */
void setTimer2Compare(uint8_t mode)
{
	/* (TCCR2A): TC2 Control Register A */
	/* (COM2xn): Compare Output Mode for Channel */
	TCCR2A |= (mode << COM2A0);
}

/* Set Timer2 Counter for Specified Time */
void setTimer2Ticks(uint16_t prescaler, uint8_t milliseconds)
{
	/* Set the duration of the timer */
	/* (OCR2A): TC2 Output Compare Register A */
	if(prescaler == 2)
		prescaler = 8;
	else if(prescaler == 3)
		prescaler = 64;
	else if(prescaler == 4)
		prescaler = 256;
	else if(prescaler == 5)
		prescaler = 1024;
	uint8_t ticks;
	ticks = (uint8_t) (((F_CPU) / (prescaler) * (milliseconds) / (1000)) - 1);
	OCR2A |= ticks;
}

/* Timer 2 Interrupt Enable
 * Sets the Interrupt corresponded to the Waveform Configuration */
void setTimer2Interrupt(uint8_t interruptWaveForm)
{
	/* Timer 2 Waveform Generation Mode 
	 * '0': Normal
	 * '1': CTC (Clear Timer on Compare Match) -> TOP: OCR2A */
	switch(interruptWaveForm)
	{
		/* (TIMSK2): Timer/Counter 2 Interrupt Mask Register */
		/* (TOIE2): Overflow Interrupt Enable */
		case 0: 
			TIMSK2 |= (1 << TOIE2);
			break;
			
		/* (OCIE2A): Output Compare A Match Interrupt Enable */		
		case 1:
			TIMSK2 |= (1 << OCIE2A);
			break;
	}
	sei();
}

void initTimer2(uint8_t waveformMode, uint8_t milliseconds, uint8_t compareMode, uint16_t prescaler, uint8_t interrupt, void (*f)())
{
	/* Set Timer 2 Waveform Mode */
	setTimer2Waveform(waveformMode);
	
	if(waveformMode)	// If Waveform Mode is CTC, set Timer 2 Ticks
		/* Set Timer 2 Ticks */
		setTimer2Ticks(prescaler, milliseconds);	
	
	/* Set Timer 2 Compare Output Mode */
	setTimer2Compare(compareMode);

	/* Configure Interrupt Mode if Enabled */
	if(interrupt)
	{
		setTimer2Interrupt(waveformMode);	// The interrupt has to match the Waveform Mode
		timer2FuncPtr = f;	// Copy f to timer1FuncPtr
	}
		
	/* Set Prescaler */
	setTimer2Prescaler(prescaler);
}

void clearTimer2()
{
	/* (TCNT2): TC2 Counter Value */
	TCNT2 = 0;	
}

/* Resets TC2 Counter Value */
void resetTimer2()
{
	/* Clears the CTC Flag (Writing a logic one to the set flag clears it) */
	/* The timer overflow is also restarted */
	TIFR2 |= (1 << TOV2);
}

/* Returns TC2 Counter Value */
uint8_t valueTimer2()
{
	/* (TCNT2): TC2 Counter Value */
	return TCNT2;
}

/* Disable Timer/Counter 2 */
void stopTimer2()
{
	/* Stop Timer/Counter by Setting 'No clock source' for Waveform */
	/* (TCCR2B): TC2 Control Register 2 B */
	/* (CS2n): Clock Select 2 [n = 0:2] */
	TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
}

/* Watch for an Overflow Event - No interruptions enabled */
uint8_t checkTimer2Overflow()
{
	/* (TIFR2): TC2 Interrupt Flag Register */
	/* (TOV2): Timer/Counter 2, Overflow Flag */
	// Wait until occurs the overflow
	if((TIFR2 & (1 << TOV2)))
		return 1;	
	return 0;
}

/* Timer 2 Interrupts, enable the desired Interrupt on the TIMER.h file */
#ifdef TIMER2_INTERRUPT_OCIEA
ISR(TIMER2_COMPA_vect)
{
	timer2FuncPtr();
}
#endif

#ifdef TIMER2_INTERRUPT_TOEI
ISR(TIMER2_OVF_vect)
{
	timer2FuncPtr();
}
#endif