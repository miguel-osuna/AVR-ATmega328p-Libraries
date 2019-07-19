/*
 * adc.h
 *
 * Created: 6/16/2019 3:24:38 AM
 * Author: Miguel Osuna
 */ 
#pragma once
#ifndef _ADC_H_
#define _ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

/*
 * //////////////////////////////////////////////////////////////////////////
 *							ADC Definitions
 * //////////////////////////////////////////////////////////////////////////
 */ 
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1 
#define ADC_CHANNEL_2 2
#define ADC_CHANNEL_3 3
#define ADC_CHANNEL_4 4 
#define ADC_CHANNEL_5 5
#define ADC_CHANNEL_6 6 
#define ADC_CHANNEL_7 7

#define ADC_PRESCALER_2 2
#define ADC_PRESCALER_4 4
#define ADC_PRESCALER_8 8
#define ADC_PRESCALER_16 16
#define ADC_PRESCALER_32 32
#define ADC_PRESCALER_64 64
#define ADC_PRESCALER_128 128

#define ADC_ADJUST_LEFT 1
#define ADC_ADJUST_RIGHT 0

#define ADC_REFERENCE_AREF 0
#define ADC_REFERENCE_ACC 1
#define ADC_REFERENCE_INTERNAL 3

#define ADC_SINGLE_MODE 0
#define ADC_RUNNING_MODE 1

/*
 * //////////////////////////////////////////////////////////////////////////
 *							ADC Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 
static inline void set_reference(uint8_t ref);
static inline void set_adjust(uint8_t adjust);
static inline void set_channel(uint8_t channel);
static inline void set_prescaler(uint8_t prescaler);
void init_adc(uint8_t prescaler, uint8_t adjust, uint8_t ref, uint8_t mode);
static inline uint16_t single_conversion_adc();
static inline uint16_t free_running_adc();
uint16_t read_adc(uint8_t channel);

#ifdef __cplusplus
}
#endif 

#endif /* _ADC_H_ */