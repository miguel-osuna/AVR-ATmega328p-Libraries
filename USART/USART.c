/*
 * USART.c
 * Created: 6/14/2019
 * Author: Miguel Osuna
 */

#include "USART.h"

void setBaudPrescaler(uint32_t cpuSpeed, uint32_t baudRate, uint8_t doubleSpeed)
{
	/* If baudRate is off the limits */
	if(baudRate <= BAUD_RATE_2400 || baudRate >= BAUD_RATE_2M)
		baudRate = 9600;
	
	uint16_t baudPrescaler;
	
	/* For Asynchronous Double Normal Mode */
	if(doubleSpeed)
		baudPrescaler = (cpuSpeed) / (8 * baudRate) - 1;
		
	/* For Asynchronous Normal Mode */
	else
		baudPrescaler = (cpuSpeed) / (16 * baudRate) - 1;

	/* USART Baud Rate Register Configuration */
	UBRR0H = (uint8_t) (baudPrescaler >> 8); // 8 first MSB
	UBRR0L = (uint8_t) (baudPrescaler >> 0); // 8 first LSB
}

void setTransmissionSpeed(uint8_t doubleSpeed)
{
	if(doubleSpeed)
		/* USART Control Status Register 0 A
		*  Writing '1' to U2X0 Doubles the USART Transmission Speed */
		UCSR0A |= (1 << U2X0);
	
	else
		/* Writing '0' to U2X0 will use synchronous operation */
		UCSR0A &= ~(1 << U2X0);
}

void enableTxRx()
{
	/* Enable USART Transmitter and Receiver
	 * (UCSR0B): USART Control Status Register 0 B */ 
	UCSR0B |= ((1 << TXEN0) | (1 << RXEN0));
}

void setFrameFormat()
{
	/* Configure 8 Data Bits and 1 Stop Bit -> 011
	 * (UCSR0C): USART Control Status Register 0 C */

	/* (USBS0): USART Stop Bit Select 0
	 * Configure to 1 stop bit */
	UCSR0C &= ~(1 << USBS0);
	
	/* (UCSZ01): USART Character Size
	 * Configure to 8 data bits -> 011 */
	UCSR0C |= ((1 << UCSZ00) | (1 << UCSZ00));
}

void initUSART(uint32_t cpuSpeed, uint32_t baudRate, uint8_t doubleSpeed)
{
	/* Sets Prescaler value to (UBRR0): USART Baud Rate Register 0 */
	setBaudPrescaler(cpuSpeed, baudRate, doubleSpeed);
	
	/* Sets USART Transmission Speed: x1 or x2 */
	setTransmissionSpeed(doubleSpeed);
	
	/* Enables USART Transmitter and USART Receiver */
	enableTxRx();
	
	/* Configuration of 8 data bits and 1 stop bit */
	setFrameFormat();
}

void transmitByte(uint8_t data)
{
	/* Wait for empty transmit buffer 
	 * (UDRE0): USART Data Register Empty 0 
	 * (UCSR0A): USART Control Status Register 0 A */ 
	while(!(UCSR0A & (1 << UDRE0)));
	
	/* The data is mounted into the
	 * (UDR0): USART Data Register 0
	 * UDR0 is a buffer for both TX0 and RX0 */
	UDR0 = data;
}

uint8_t receiveByte()
{
	/* Wait for incoming data
	 * (UCSR0A): USART Control Status Register 0 A 
	 * (RXC0): USART Receive Complete */
	while(!(UCSR0A & (1 << RXC0)));
	
	/* Return value stored in Buffer */
	return UDR0;
}

void printByte(uint8_t number)
{
	/* We add '0' to make it an ASCII character */
	
	// Hundreds
	transmitByte('0' + (number / 100));
	
	// Tens
	transmitByte('0' + ((number / 10) % 10));
	
	// Units
	transmitByte('0' + (number % 10));
}

void putString(const char *str)
{
	for(uint8_t i = 0; *(str + i) != '\0'; i++)
		transmitByte(*(str + i));
}

void getString (char *str, uint8_t size)
{
	uint8_t i = 0;
	uint8_t c;
	do
	{
		c = receiveByte();
		if(c != 13) // If char is different from 'Enter'
		{
			*(str + i) = c;
			i++;
		}
	} while (c != 13 && i < size);
	
	*(str + i) = '\0';
}

void printNumber(uint16_t number)
{
	char buffer[5];
	// number, string, base
	itoa(number, buffer, 10);
	putString(buffer);
}

void printLine()
{
	putString("\n");
}