/*
 * timer.c
 *
 * Created: 6/20/2019
 * Author: Miguel Osuna
 */ 

#include "timer.h"

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Timer/Counter 1 Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 

/* 
 * Timer/set_timer1_ticks 1 Prescaler
 * '0': No clock source (Timer/Counter stopped)
 * '1': clk/1 (No prescaling)
 * '2': clk/8 (From prescaler)
 * '3': clk/64 (From prescaler)
 * '4': clk/256 (From prescaler)
 * '5': clk/1024 (From prescaler) 
 */
static inline void set_timer1_prescaler(uint16_t prescaler)
{
	// Start the Timer, by Setting the Prescaler
	// (TCCR1B): TC1 Control Register 1 B */
	// (CS1n): Clock Select 1 [n = 0:2] */
	TCCR1B |= (prescaler << CS10);	
}

/*  
 * Timer 1 Waveform Generation Mode 
 * '0': Normal
 * '1': CTC (Clear Timer on Compare Match) -> TOP: OCR1A
 * '2': CTC (Clear Timer on Compare Match) -> TOP: ICR1 
 */
static inline void set_timer1_waveform(uint8_t waveform)
{
	switch(waveform)
	{
		// (TCCR1A): TC1 Control Register A
		// (TCCR1B): TC1 Control Register B
		// (WGMnn): Waveform Generation Mode
		// Normal Mode
		case WAVEFORM_NORMAL:
			TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
			TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
			break;	
			
		// CTC - TOP: OCR1A
		case WAVEFORM_CTC_OCR1A:
			TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
			TCCR1B |= (1 << WGM12);
			TCCR1B &= ~(1 << WGM13);
			break;	
			
		// CTC - TOP: ICR1
		case WAVEFORM_CTC_ICR1:
			TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
			TCCR1B |= ((1 << WGM13) | (1 << WGM12));
			break;	
			
		// Normal Mode by Default
		default:
			TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
			TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
			break;
	}
}

/* 
 * Timer 1 Compare Output Mode for Channel (non-PWM) 
 * '0': Normal port operation, OC1A/OC1B disconnected
 * '1': Toggle OC1A/OC1B on Compare Match
 * '2': Clear OC1A/OC1B on Compare Match
 * '3': Set OC1A/OC2B on Compare Match
 */
static inline void set_timer1_compare(uint8_t mode)
{
	// (TCCR1A): TC1 Control Register A
	// (COM1xn): Compare Output Mode for Channel
	TCCR1A |= (mode << COM1A0);
}

// Set Timer1 Counter for Specified Time
static inline void set_timer1_ticks(uint16_t prescaler, uint16_t msec)
{
	// Set the duration of the timer
	// (OCR1A): TC1 Output Compare Register A
	switch(prescaler)
	{
		case TIMER1_PRESCALER_8: 
			prescaler = 8;
			break;

		case TIMER1_PRESCALER_64:
			prescaler = 64;
			break;

		case TIMER1_PRESCALER_256:
			prescaler = 256;
			break;

		case TIMER1_PRESCALER_1024:
			prescaler = 1024;
			break;

		default:
			prescaler = 1;
			break;
	}

	uint16_t ticks; // Up to 65535
	ticks = (uint16_t) (((F_CPU) / (prescaler) * (msec) / (1000)) - 1);
	OCR1A |= ticks;
}

/* 
 * Timer 1 Interrupt Enable
 * Sets the Interrupt corresponded to the Waveform Configuration 
 */
static inline void set_timer1_interrupt(uint8_t interrupt)
{
	/* 
	 * Waveform Mode 
	 * '0': Normal
	 * '1': CTC (Clear Timer on Compare Match) -> TOP: OCR1A
	 * '2': CTC (Clear Timer on Compare Match) -> TOP: ICR1 
	 */ 	
	switch(interrupt)
	{
		// (TIMSK1): Timer/Counter 1 Interrupt Mask Register
		// (TOIE1): Overflow Interrupt Enable 1
		case WAVEFORM_NORMAL: 
			TIMSK1 |= (1 << TOIE1);
			break;
			
		// (OCIE1A): Output Compare A Match Interrupt Enable 1
		case WAVEFORM_CTC_OCR1A:
			TIMSK1 |= (1 << OCIE1A);
			break;
			
		// (ICIE): Input Capture Interrupt Enable 1
		case WAVEFORM_CTC_ICR1:
			TIMSK1 |= set_timer0_waveform (1 << ICIE1);
			break;

		default:
			TIMSK1 |= (1 << TOIE1);
	}

	sei();
}

/* Initialize Timer 1 */
void init_timer1(uint8_t waveform_mode, uint16_t msec, uint8_t compare_mode, uint16_t prescaler, uint8_t interrupt, void (*f)())
{
	// Set Timer 1 Waveform Mode
	set_timer1_waveform(waveform_mode);
	
	if(waveform_mode) // If Waveform Mode is CTC, set Timer 1 Ticks
		// Set Timer 1 Ticks
		set_timer1_ticks(prescaler, msec);	
	
	// Set Timer 1 Compare Output Mode
	set_timer1_compare(compareMode);

	// Configure Interrupt Mode if Enabled
	if(interrupt)
	{
		set_timer1_interrupt(waveform_mode); // The interrupt has to match the Waveform Mode
		gp_timer1_func = f;				  // Copy f to gp_timer1_func
	}
		
	// Set Prescaler
	set_timer1_prescaler(prescaler);
}

// Returns TC1 Counter Value
uint16_t value_timer1()
{
	// (TCNT1): TC1 Counter Value
	return TCNT1;
} 

// Sets TC1 to 0
void clear_timer1()
{
	// (TCNT1): TC1 Counter Value
	TCNT1 = 0;	
}

// Resets TC1 Counter Value
void reset_timer1()
{
	// Clears the CTC Flag (Writing a logic one to the set flag clears it)
	// The timer overflow is also restarted
	TIFR1 |= (1 << TOV1);
}

// Disable Timer/Counter 1
void stop_timer1()
{
	// Stop Timer/Counter by Setting 'No clock source' for Waveform
	// (TCCR1B): TC1 Control Register 1 B
	// (CS1n): Clock Select 1 [n = 0:2]
	TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

/* Watch for an Overflow Event - No interruptions enabled */
uint8_t check_timer1_overflow()
{
	uint8_t b_overflow;
	// (TIFR1): TC1 Interrupt Flag Register
	// (TOV1): Timer/Counter 1, Overflow Flag
	if((TIFR1 & (1 << TOV1)))	// Wait until occurs the overflow
		b_overflow = 1;	
	b_overflow = 0;
	return b_overflow;
}

/* Timer 1 Interrupts, enable the desired Interrupt on the TIMER.h file */
#ifdef TIMER1_INTERRUPT_OCIEA
ISR(TIMER1_COMPA_vect)
{
	gp_timer1_func();
}
#endif

#ifdef TIMER1_INTERRUPT_TOEI
ISR(TIMER1_OVF_vect)
{
	gp_timer1_func();
}
#endif

#ifdef TIMER1_INTERRUPT_ICIE
ISR(TIMER1_CAPT_vect)
{
	gp_timer1_func();
}
#endif

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Timer/Counter 0 Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 

/* 
 * Timer/Counter 0 Prescaler
 * '0': No clock source (Timer/Counter stopped)
 * '1': clk/1 (No prescaling)
 * '2': clk/8 (From prescaler)
 * '3': clk/64 (From prescaler)
 * '4': clk/256 (From prescaler)
 * '5': clk/1024 (From prescaler) 
 */
static inline void set_timer0_prescaler(uint16_t prescaler)
{
	// Start the Timer, by Setting the Prescaler
	// (TCCR0B): TC0 Control Register 0 B
	// (CS0n): Clock Select 0 [n = 0:2]
	TCCR0B |= (prescaler << CS00);	
}

/* 
 * Timer 0 Waveform Generation Mode 
 * '0': Normal
 * '1': CTC (Clear Timer on Compare Match) -> TOP: OCR0A
 */
static inline void set_timer0_waveform(uint8_t waveform)
{
	switch(waveform)
	{
		// (TCCR0A): TC0 Control Register A
		// (TCCR0B): TC0 Control Register B
		// (WGM0n): Waveform Generation Mode
		// Normal Mode
		case WAVEFORM_NORMAL:
			TCCR1A &= ~((1 << WGM01) | (1 << WGM00));
			TCCR1B &= ~((1 << WGM02));
			break;	
			
		// CTC - TOP: OCR0A
		case WAVEFORM_CTC_OCR0A:
			TCCR1A &= ~((1 << WGM00));
			TCCR1A |= (1 << WGM01);
			TCCR1B &= ~(1 << WGM02);
			break;	
			
		// Normal Mode by Default
		default:
			TCCR1A &= ~((1 << WGM01) | (1 << WGM00));
			TCCR1B &= ~((1 << WGM02));
			break;
	}
}

/*
 * Timer 0 Compare Output Mode for Channel (non-PWM) 
 * '0': Normal port operation, OC0A disconnected
 * '1': Toggle OC0A on Compare Match
 * '2': Clear OC0A on Compare Match
 * '3': Set OC0A on Compare Match
 */
static inline void set_timer0_compare(uint8_t mode)
{
	// (TCCR0A): TC0 Control Register A
	// (COM0xn): Compare Output Mode for Channel
	TCCR0A |= (mode << COM0A0);
}

/* Set Timer0 Counter for Specified Time */
static inline void set_timer0_ticks(uint16_t prescaler, uint8_t msec)
{
	// Set the duration of the timer
	// (OCR0A): TC0 Output Compare Register A
	switch(prescaler)
	{
		case TIMER0_PRESCALER_8:
			prescaler = 8;
			break;

		case TIMER0_PRESCALER_64:
			prescaler = 64;
			break;

		case TIMER0_PRESCALER_256:
			prescaler = 256;
			break;

		case TIMER0_PRESCALER_1024:
			prescaler = 1024;
			break;

		default:
			prescaler = 1;
			break;
	}

	uint8_t ticks;
	ticks = (uint8_t) (((F_CPU) / (prescaler) * (msec) / (1000)) - 1);
	OCR0A |= ticks;
}

/* 
 * Timer 0 Interrupt Enable
 * Sets the Interrupt corresponded to the Waveform Configuration 
 */
static inline void set_timer0_interrupt(uint8_t interrupt)
{
	/* 
	 * Timer 0 Waveform Generation Mode 
	 * '0': Normal
	 * '1': CTC (Clear Timer on Compare Match) -> TOP: OCR0A 
	 */
	switch(interrupt)
	{
		// (TIMSK0): Timer/Counter 0 Interrupt Mask Register
		// (TOIE0): Overflow Interrupt Enable
		case WAVEFORM_NORMAL: 
			TIMSK0 |= (1 << TOIE0);
			break;
			
		// (OCIE0A): Output Compare A Match Interrupt Enable
		case WAVEFORM_CTC_OCR0A:
			TIMSK0 |= (1 << OCIE0A);
			break;

		// Default
		default:
			TIMSK0 |= (1 << TOIE0);
			break;
	}

	sei();
}

/* Initialize Timer 0 */
void init_timer0(uint8_t waveform_mode, uint8_t msec, uint8_t compareMode, uint16_t prescaler, uint8_t interrupt, void (*f)())
{
	// Set Timer 0 Waveform Mode
	set_timer0_waveform(waveform_mode);
	
	if(waveform_mode)	// If Waveform Mode is CTC, set Timer 0 Ticks
		// Set Timer 0 Ticks
		set_timer0_ticks(prescaler, msec);	
	
	// Set Timer 0 Compare Output Mode
	set_timer0_compare(compareMode);

	// Configure Interrupt Mode if Enabled
	if(interrupt)
	{
		set_timer0_interrupt(waveform_mode);	// The interrupt has to match the Waveform Mode
		gp_timer0_func = f;	// Copy f to gp_timer0_func
	}
		
	// Set Prescaler
	set_timer0_prescaler(prescaler);
}

/* Returns TC0 Counter Value */
uint8_t value_timer0()
{
	// (TCNT0): TC0 Counter Value
	return TCNT0;
}

/* Set TC0 to 0 */
void clear_timer0()
{
	// (TCNT0): TC0 Counter Value
	TCNT0 = 0;	
}

/* Resets TC0 Counter Value */
void reset_timer0()
{
	// Clears the CTC Flag (Writing a logic one to the set flag clears it)
	// The timer overflow is also restarted
	TIFR0 |= (1 << TOV0);
}

/* Disable Timer/Counter 0 */
void stopTimer0()
{
	// Stop Timer/Counter by Setting 'No clock source' for Waveform
	// (TCCR0B): TC0 Control Register 0 B
	// (CS0n): Clock Select 0 [n = 0:2]
	TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}

/* Watch for an Overflow Event - No interruptions enabled */
uint8_t check_timer0_overflow()
{
	uint8_t b_overflow;
	// (TIFR0): TC0 Interrupt Flag Register
	// (TOV0): Timer/Counter 0, Overflow Flag
	// Wait until occurs the overflow
	if((TIFR0 & (1 << TOV0)))
		b_overflow = 1;	
	b_overflow = 0;
	return b_overflow;
}

/* Timer 0 Interrupts, enable the desired Interrupt on the TIMER.h file */
#ifdef TIMER0_INTERRUPT_OCIEA
ISR(TIMER0_COMPA_vect)
{
	gp_timer0_func();
}
#endif

#ifdef TIMER0_INTERRUPT_TOEI
ISR(TIMER0_OVF_vect)
{
	gp_timer0_func();
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
static inline void set_timer2_prescaler(uint16_t prescaler)
{
	// Start the Timer, by Setting the Prescaler
	// (TCCR2B): TC0 Control Register 2 B
	// (CS2n): Clock Select 2 [n = 0:2]
	TCCR2B |= (prescaler << CS20);	
}

/* 
 * Timer 2 Waveform Generation Mode 
 * '0': Normal
 * '1': CTC (Clear Timer on Compare Match) -> TOP: OCR2A
 */
static inline void set_timer2_waveform(uint8_t waveform)
{
	switch(waveform)
	{
		// (TCCR2A): TC2 Control Register A
		// (TCCR2B): TC2 Control Register B
		// (WGM2n): Waveform Generation Mode
		// Normal Mode
		case WAVEFORM_NORMAL:
			TCCR2A &= ~((1 << WGM21) | (1 << WGM20));
			TCCR2B &= ~((1 << WGM22));
			break;	
			
		// CTC - TOP: OCR2A 
		case WAVEFORM_CTC_OCR2A:
			TCCR2A &= ~((1 << WGM20));
			TCCR2A |= (1 << WGM21);
			TCCR2B &= ~(1 << WGM22);
			break;	
			
		// Normal Mode by Default
		default:
			TCCR2A &= ~((1 << WGM21) | (1 << WGM20));
			TCCR2B &= ~((1 << WGM22));
			break;
	}
}

/* 
 * Timer 2 Compare Output Mode for Channel (non-PWM) 
 * '0': Normal port operation, OC2A disconnected
 * '1': Toggle OC2A on Compare Match
 * '2': Clear OC2A on Compare Match
 * '3': Set OC2A on Compare Match 
 */
static inline void set_timer2_compare(uint8_t mode)
{
	// (TCCR2A): TC2 Control Register A
	// (COM2xn): Compare Output Mode for Channel
	TCCR2A |= (mode << COM2A0);
}

/* Set Timer2 Counter for Specified Time */
static inline void set_timer2_ticks(uint16_t prescaler, uint8_t msec)
{
	// Set the duration of the timer
	// (OCR2A): TC2 Output Compare Register A 
	switch (prescaler) 
	{
		case TIMER2_PRESCALER_8:
			prescaler = 8;
			break;

		case TIMER2_PRESCALER_64:
			prescaler = 64;
			break;

		case TIMER2_PRESCALER_256:
			prescaler = 256;
			break;

		case TIMER2_PRESCALER_1024:
			prescaler = 1024;
			break;

		default:
			prescaler = 1;
			break;
	}

	uint8_t ticks;
	ticks = (uint8_t) (((F_CPU) / (prescaler) * (msec) / (1000)) - 1);
	OCR2A |= ticks;
}

/*
 *  Timer 2 Interrupt Enable
 * Sets the Interrupt corresponded to the Waveform Configuration 
 */
static inline void set_timer2_interrupt(uint8_t interrupt)
{
	/* 
	 * Timer 2 Waveform Generation Mode 
	 * '0': Normal
	 * '1': CTC (Clear Timer on Compare Match) -> TOP: OCR2A 
	 */
	switch(interrupt)
	{
		// (TIMSK2): Timer/Counter 2 Interrupt Mask Register
		// (TOIE2): Overflow Interrupt Enable
		case WAVEFORM_NORMAL: 
			TIMSK2 |= (1 << TOIE2);
			break;
			
		// (OCIE2A): Output Compare A Match Interrupt Enable
		case WAVEFORM_CTC_OCR2A:
			TIMSK2 |= (1 << OCIE2A);
			break;
	}

	sei();
}

void init_timer2(uint8_t waveform_mode, uint8_t msec, uint8_t compareMode, uint16_t prescaler, uint8_t interrupt, void (*f)())
{
	// Set Timer 2 Waveform Mode
	set_timer2_waveform(waveform_mode);
	
	if(waveform_mode)	// If Waveform Mode is CTC, set Timer 2 Ticks
		// Set Timer 2 Ticks
		set_timer2_ticks(prescaler, msec);	
	
	// Set Timer 2 Compare Output Mode
	set_timer2_compare(compareMode);

	// Configure Interrupt Mode if Enabled
	if(interrupt)
	{
		set_timer2_interrupt(waveform_mode);	// The interrupt has to match the Waveform Mode
		gp_timer2_func = f;	// Copy f to gp_timer2_func
	}
		
	// Set Prescaler
	set_timer2_prescaler(prescaler);
}

/* Returns TC2 Counter Value */
uint8_t value_timer2()
{
	// (TCNT2): TC2 Counter Value
	return TCNT2;
}

/* Clears Timer 2 */
void clear_timer2()
{
	// (TCNT2): TC2 Counter Value
	TCNT2 = 0;	
}

/* Resets TC2 Counter Value */
void reset_timer2()
{
	// Clears the CTC Flag (Writing a logic one to the set flag clears it)
	// The timer overflow is also restarted
	TIFR2 |= (1 << TOV2);
}

/* Disable Timer/Counter 2 */
void stop_timer2()
{
	// Stop Timer/Counter by Setting 'No clock source' for Waveform 
	// (TCCR2B): TC2 Control Register 2 B
	// (CS2n): Clock Select 2 [n = 0:2]
	TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
}

/* Watch for an Overflow Event - No interruptions enabled */
uint8_t check_timer2_overflow()
{
	uint8_t b_overflow;
	// (TIFR2): TC2 Interrupt Flag Register
	// (TOV2): Timer/Counter 2, Overflow Flag
	// Wait until occurs the overflow
	if((TIFR2 & (1 << TOV2)))
		b_overflow =  1;	
	b_overflow = 0;
	return b_overflow;
}

/* Timer 2 Interrupts, enable the desired Interrupt on the TIMER.h file */
#ifdef TIMER2_INTERRUPT_OCIEA
ISR(TIMER2_COMPA_vect)
{
	gp_timer2_func();
}
#endif

#ifdef TIMER2_INTERRUPT_TOEI
ISR(TIMER2_OVF_vect)
{
	gp_timer2_func();
}
#endif