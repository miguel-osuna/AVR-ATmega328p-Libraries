/*
 * ADC.h
 *
 * Created: 6/16/2019 3:24:38 AM
 *  Author: Miguel Osuna
 */ 

#pragma once
#ifndef _ADC_H_
#define _ADC_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef CHANNEL
#define CHANNEL 0
#endif

#ifndef PRESCALER
#define PRESCALER 128
#endif

#ifndef ADJUST
#define ADJUST 0
#endif

#ifndef REFERENCE
#define REFERENCE 1
#endif

#ifndef MODE
#define MODE 0
#endif 

#include <avr/io.h>
#include <util/delay.h>

void setReference(uint8_t ref);
void setAdjust(uint8_t adjust);
void setChannel(uint8_t channel);
void setPrescaler(uint8_t prescaler);
void initADC(uint8_t prescaler, uint8_t adjust, uint8_t ref, uint8_t mode);
uint16_t singleConversionADC();
uint16_t freeRunningADC();
uint16_t readADC(uint8_t channel);

#endif /* _ADC_H_ */