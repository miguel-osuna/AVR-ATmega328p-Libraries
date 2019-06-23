/*
 * TIMER.h
 *
 * Created: 6/20/2019
 * Author: Miguel Osuna
 */ 

#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* Defines for Timer 1 */
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

/* Defines for Timer 0 */
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

/* Defines for Timer 2 */
#define TIMER2_PRESCALER_NONE 0
#define TIMER2_PRESCALER_1 1
#define TIMER2_PRESCALER_8 2
#define TIMER2_PRESCALER_64 3
#define TIMER2_PRESCALER_256 4
#define TIMER2_PRESCALER_1024 5

#define WAVEFORM_NORMAL 0
#define WAVEFORM_CTC_OCR1A 1
#define WAVEFORM_CTC_ICR1 2

#define COMPARE_NORMAL 0
#define COMPARE_TOGGLE 1
#define COMPARE_CLEAR 2
#define COMPARE_SET 3

#define TIMER2_INTERRUPT_ICIE 0
#define TIMER2_INTERRUPT_OCIEA 1
#define TIMER2_INTERRUPT_TOEI 0

/* Timer 1 Functions */
void (*timer1FuncPtr)(); // Global Function Pointer
void setTimer1Prescaler(uint16_t prescaler);
void setTimer1Waveform(uint8_t waveform);
void setTimer1Compare(uint8_t mode);
void setTimer1Ticks(uint16_t prescaler, uint16_t milliseconds);
void setTimer1Interrupt(uint8_t interrupt);
void initTimer1(uint8_t waveformMode, uint16_t milliseconds, uint8_t compareMode, uint16_t prescaler, uint8_t interrupt, void (*f)() );
uint16_t valueTimer1();
void clearTimer1();
void resetTimer1();
void stopTimer1();
uint8_t checkTimer1Overflow();

/* Timer 0 Functions */
void (*timer0FuncPtr)(); // Global Function Pointer
void setTimer0Prescaler(uint16_t prescaler);
void setTimer0Waveform(uint8_t waveform);
void setTimer0Compare(uint8_t mode);
void setTimer0Ticks(uint16_t prescaler, uint8_t milliseconds);
void setTimer0Interrupt(uint8_t interrupt);
void initTimer0(uint8_t waveformMode, uint8_t milliseconds, uint8_t compareMode, uint16_t prescaler, uint8_t interrupt, void (*f)() );
uint8_t valueTimer0();
void clearTimer0();
void resetTimer0();
void stopTimer0();
uint8_t checkTimer0Overflow();

/* Timer 2 Functions */
void (*timer2FuncPtr)(); // Global Function Pointer
void setTimer2Prescaler(uint16_t prescaler);
void setTimer2Waveform(uint8_t waveform);
void setTimer2Compare(uint8_t mode);
void setTimer2Ticks(uint16_t prescaler, uint8_t milliseconds);
void setTimer2Interrupt(uint8_t interrupt);
void initTimer2(uint8_t waveformMode, uint8_t milliseconds, uint8_t compareMode, uint16_t prescaler, uint8_t interrupt, void (*f)() );
uint8_t valueTimer2();
void clearTimer2();
void resetTimer2();
void stopTimer2();
uint8_t checkTimer2Overflow();

#endif /* TIMER_H_ */