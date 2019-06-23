/*
 * Interrupt.h
 *
 * Created: 6/18/2019 3:24:23 AM
 *  Author: Miguel Osuna
 */ 

#pragma once
#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

void enableINT(uint8_t interrupt, uint8_t mode);
void enablePCIE(uint8_t port, uint8_t interrupt);

#endif /* INTERRUPT_H_ */