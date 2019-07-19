/*
 * timer.h
 *
 * Created: 6/20/2019
 * Author: Miguel Osuna
 */ 

#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_

#ifndef __cplusplus
extern "C" {
#endif

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Timer/Counter 1 Definitions
 * //////////////////////////////////////////////////////////////////////////
 */ 
#define TIMER1_PRESCALER_NONE 0
#define TIMER1_PRESCALER_1 1
#define TIMER1_PRESCALER_8 2
#define TIMER1_PRESCALER_64 3
#define TIMER1_PRESCALER_256 4
#define TIMER1_PRESCALER_1024 5

#define WAVEFORM_NORMAL 0 
#define WAVEFORM_CTC_OCR1A 1
#define WAVEFORM_CTC_ICR1 2

#define COMPARE_NORMAL 0
#define COMPARE_TOGGLE 1
#define COMPARE_CLEAR 2
#define COMPARE_SET 3

#define TIMER1_INTERRUPT_ICIE 0
#define TIMER1_INTERRUPT_OCIEA 1
#define TIMER1_INTERRUPT_TOEI 0

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Timer/Counter 0 Definitions
 * //////////////////////////////////////////////////////////////////////////
 */ 
#define TIMER0_PRESCALER_NONE 0
#define TIMER0_PRESCALER_1 1
#define TIMER0_PRESCALER_8 2
#define TIMER0_PRESCALER_64 3
#define TIMER0_PRESCALER_256 4
#define TIMER0_PRESCALER_1024 5

#define WAVEFORM_NORMAL 0
#define WAVEFORM_CTC_OCR0A 1
#define WAVEFORM_CTC_ICR0 2

#define COMPARE_NORMAL 0
#define COMPARE_TOGGLE 1
#define COMPARE_CLEAR 2
#define COMPARE_SET 3

#define TIMER0_INTERRUPT_OCIEA 1
#define TIMER0_INTERRUPT_TOEI 0

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Timer/Counter 2 Definitions
 * //////////////////////////////////////////////////////////////////////////
 */ 
#define TIMER2_PRESCALER_NONE 0
#define TIMER2_PRESCALER_1 1
#define TIMER2_PRESCALER_8 2
#define TIMER2_PRESCALER_64 3
#define TIMER2_PRESCALER_256 4
#define TIMER2_PRESCALER_1024 5

#define WAVEFORM_NORMAL 0
#define WAVEFORM_CTC_OCR2A 1
#define WAVEFORM_CTC_ICR2 2

#define COMPARE_NORMAL 0
#define COMPARE_TOGGLE 1
#define COMPARE_CLEAR 2
#define COMPARE_SET 3

#define TIMER2_INTERRUPT_ICIE 0
#define TIMER2_INTERRUPT_OCIEA 1
#define TIMER2_INTERRUPT_TOEI 0

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Timer/Counter 1 Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 
void (*gp_timer1_func)(); // Global Function Pointer
static inline void set_timer1_prescaler(uint16_t prescaler);
static inline void set_timer1_waveform(uint8_t waveform);
static inline void set_timer1_compare(uint8_t mode);
static inline void set_timer1_ticks(uint16_t prescaler, uint16_t msec);
static inline void set_timer1_interrupt(uint8_t interrupt);
void init_timer1(uint8_t waveform_mode, uint16_t msec, uint8_t compare_mode, uint16_t prescaler, uint8_t interrupt, void (*f)() );
uint16_t value_timer1();
void clear_timer1();
void reset_timer1();
void stop_timer1();
uint8_t check_timer1_overflow();

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Timer/Counter 0 Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 
void (*gp_timer0_func)(); // Global Function Pointer
static inline void set_timer0_prescaler(uint16_t prescaler);
static inline void set_timer0_waveform(uint8_t waveform);
static inline void set_timer0_compare(uint8_t mode);
static inline void set_timer0_ticks(uint16_t prescaler, uint8_t msec);
static inline void set_timer0_interrupt(uint8_t interrupt);
void init_timer0(uint8_t waveform_mode, uint8_t msec, uint8_t compare_mode, uint16_t prescaler, uint8_t interrupt, void (*f)() );
uint8_t value_timer0();
void clear_timer0();
void reset_timer0();
void stop_timer0();
uint8_t check_timer0_overflow();

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Timer/Counter 2 Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 
void (*gp_timer2_func)(); // Global Function Pointer
static inline void set_timer2_prescaler(uint16_t prescaler);
static inline void set_timer2_waveform(uint8_t waveform);
static inline void set_timer2_compare(uint8_t mode);
static inline void set_timer2_ticks(uint16_t prescaler, uint8_t msec);
static inline void set_timer2_interrupt(uint8_t interrupt);
void init_timer2(uint8_t waveform_mode, uint8_t msec, uint8_t compare_mode, uint16_t prescaler, uint8_t interrupt, void (*f)() );
uint8_t value_timer2();
void clear_timer2();
void reset_timer2();
void stop_timer2();
uint8_t check_timer2_overflow();

#ifdef __cplusplus
}
#endif 

#endif /* _TIMER_H_ */