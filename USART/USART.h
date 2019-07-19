/*
 * usart.h
 * Created: 6/14/2019
 * Author: Miguel Osuna
 */ 

#pragma once
#ifndef _USART_H_
#define _USART_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdlib.h>

/*
 * //////////////////////////////////////////////////////////////////////////
 *							USART Definitions
 * //////////////////////////////////////////////////////////////////////////
 */ 

#define USART_BAUD_RATE_2400 2400
#define USART_BAUD_RATE_4800 4800
#define USART_BAUD_RATE_9600 9600
#define USART_BAUD_RATE_14K4 14400
#define USART_BAUD_RATE_19K3 19200
#define USART_BAUD_RATE_28K8 28800
#define USART_BAUD_RATE_38K4 38400
#define USART_BAUD_RATE_57K6 57600
#define USART_BAUD_RATE_76K8 76800
#define USART_BAUD_RATE_115K2 115200
#define USART_BAUD_RATE_230K4 230400
#define USART_BAUD_RATE_250K 250000
#define USART_BAUD_RATE_500K 500000
#define USART_BAUD_RATE_1M 1000000
#define USART_BAUD_RATE_2M 2000000

/*
 * //////////////////////////////////////////////////////////////////////////
 *							USART Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 
void init_usart(uint32_t cpu_speed, uint32_t baud_rate, uint8_t double_speed);

/* RX and TX Functions */
void tx_byte(uint8_t data);
uint8_t rx_byte();
void print_byte(uint8_t number);
void put_string(const char *str);
void get_string(char *str, uint8_t size);
void print_number(uint16_t number);
void print_line();

#endif /* _USART_H_ */