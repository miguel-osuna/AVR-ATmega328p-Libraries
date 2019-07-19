/*
 * interrupt.h
 *
 * Created: 6/18/2019 3:24:23 AM
 * Author: Miguel Osuna
 */ 

#pragma once
#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#ifndef __cplusplus
extern "C" {
#endif

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Interrupt Definitions
 * //////////////////////////////////////////////////////////////////////////
 */ 
// External Interrupt
#define EXTERNAL_INTERRUPT_REQUEST_0 INT0
#define EXTERNAL_INTERRUPT_REQUEST_1 INT1

// Pin Change Interrupt
#define PIN_CHANGE_INTERRUPT_0 PCINT0 // PORTB
#define PIN_CHANGE_INTERRUPT_1 PCINT1 // PORTC
#define PIN_CHANGE_INTERRUPT_2 PCINT2 // PORTD

// Interrupt Sense Control Mode
#define INTERRUPT_MODE_LOW 0
#define INTERRUPT_MODE_LOGICAL_CHANGE 1
#define INTERRUPT_MODE_FALLING_EDGE 2
#define INTERRUPT_MODE_RISING_EDGE 3

/*
 * //////////////////////////////////////////////////////////////////////////
 *						Interrupt Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 
void enable_interrupt(uint8_t interrupt, uint8_t mode);
void enable_pcie(uint8_t port, uint8_t interrupt_pin);

#ifdef __cplusplus
}
#endif 

#endif /* _INTERRUPT_H_ */