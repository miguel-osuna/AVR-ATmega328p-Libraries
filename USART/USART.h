/*
 * USART.h
 * Created: 6/14/2019
 * Author: Miguel Osuna
 */ 

#pragma once
#ifndef _USART_H_
#define _USART_H_

/* Defines for USART */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUD_RATE_2400 2400
#define BAUD_RATE_4800 4800
#define BAUD_RATE_9600 9600
#define BAUD_RATE_14K4 14400
#define BAUD_RATE_19K3 19200
#define BAUD_RATE_28K8 28800
#define BAUD_RATE_38K4 38400
#define BAUD_RATE_57K6 57600
#define BAUD_RATE_76K8 76800
#define BAUD_RATE_115K2 115200
#define BAUD_RATE_230K4 230400
#define BAUD_RATE_250K 250000
#define BAUD_RATE_500K 500000
#define BAUD_RATE_1M 1000000
#define BAUD_RATE_2M 2000000

#include <avr/io.h>
#include <stdlib.h>
//#include <util/setbaud.h>

/* Configuration Functions */
void setBaudPrescaler(uint32_t cpuSpeed, uint32_t baudRate, uint8_t doubleSpeed);
void setTransmissionSpeed(uint8_t doubleSpeed);
void enableTxRx();
void setFrameFormat();
void initUSART(uint32_t cpuSpeed, uint32_t baudRate, uint8_t doubleSpeed);

/* RX and TX Functions */
void transmitByte(uint8_t data);
uint8_t receiveByte();
void printByte(uint8_t number);
void putString(const char *str);
void getString(char *str, uint8_t size);
void printNumber(uint16_t number);
void printLine();

#endif /* _USART_H_ */