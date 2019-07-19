/*
 * usart.c
 * Created: 6/14/2019
 * Author: Miguel Osuna
 */

#include "usart.h"

/*
 * //////////////////////////////////////////////////////////////////////////
 *								USART Functions
 * //////////////////////////////////////////////////////////////////////////
 */ 

/* Set USART Baud Prescaler */
static void set_baud_prescaler(uint32_t cpu_speed, uint32_t baud_rate, uint8_t double_speed)
{
	// If baud_rate is off the limits
	if(baud_rate <= USART_BAUD_RATE_2400 || baud_rate >= USART_BAUD_RATE_2M)
		baud_rate = 9600;
	
	uint16_t baud_prescaler;
	
	// For Asynchronous Double Normal Mode
	if(double_speed)
		baud_prescaler = (cpu_speed) / (8 * baud_rate) - 1;
		
	// For Asynchronous Normal Mode
	else
		baud_prescaler = (cpu_speed) / (16 * baud_rate) - 1;

	// USART Baud Rate Register Configuration
	UBRR0H = (uint8_t) (baud_prescaler >> 8); // 8 first MSB
	UBRR0L = (uint8_t) (baud_prescaler >> 0); // 8 first LSB
}

/* Set Transmission Speed */
static void set_tx_speed(uint8_t double_speed)
{
	if(double_speed)
		// USART Control Status Register 0 A
		// Writing '1' to U2X0 Doubles the USART Transmission Speed
		UCSR0A |= (1 << U2X0);
	
	else
		// Writing '0' to U2X0 will use synchronous operation
		UCSR0A &= ~(1 << U2X0);
}

/* Enable USART Transmitter and Receiver */
static void enable_tx_rx()
{
	// (UCSR0B): USART Control Status Register 0 B
	UCSR0B |= ((1 << TXEN0) | (1 << RXEN0));
}

/* Set Frame Format */
static void set_frame_format()
{
	// Configure 8 Data Bits and 1 Stop Bit -> 011
	// (UCSR0C): USART Control Status Register 0 C

	// (USBS0): USART Stop Bit Select 0
	// Configure to 1 stop bit
	UCSR0C &= ~(1 << USBS0);
	
	// (UCSZ01): USART Character Size
	// Configure to 8 data bits -> 011
	UCSR0C |= ((1 << UCSZ00) | (1 << UCSZ00));
}

/* Initialize USART Configuration */
void init_usart(uint32_t cpu_speed, uint32_t baud_rate, uint8_t double_speed)
{
	// Sets Prescaler value to (UBRR0): USART Baud Rate Register 0
	set_baud_prescaler(cpu_speed, baud_rate, double_speed);
	
	// Sets USART Transmission Speed: x1 or x2
	set_tx_speed(double_speed);
	
	// Enables USART Transmitter and USART Receiver
	enable_tx_rx();
	
	// Configuration of 8 data bits and 1 stop bit
	set_frame_format();
}

/* Transmit Byte */
void tx_byte(uint8_t data)
{
	// Wait for empty transmit buffer 
	// (UDRE0): USART Data Register Empty 0 
	// (UCSR0A): USART Control Status Register 0 A
	while(!(UCSR0A & (1 << UDRE0)));
	
	// The data is mounted into the
	// (UDR0): USART Data Register 0
	// UDR0 is a buffer for both TX0 and RX0
	UDR0 = data;
}

/* Receive Byte */
uint8_t rx_byte()
{
	/* Wait for incoming data
	 * (UCSR0A): USART Control Status Register 0 A 
	 * (RXC0): USART Receive Complete */
	while(!(UCSR0A & (1 << RXC0)));
	
	/* Return value stored in Buffer */
	return UDR0;
}

/* Print Byte */
void print_byte(uint8_t number)
{
	/* We add '0' to make it an ASCII character */
	
	// Hundreds
	tx_byte('0' + (number / 100));
	
	// Tens
	tx_byte('0' + ((number / 10) % 10));
	
	// Units
	tx_byte('0' + (number % 10));
}

/* Put String */
void put_string(const char *str)
{
	for(uint8_t i = 0; *(str + i) != '\0'; i++)
		tx_byte(*(str + i));
}

/* Get String */
void get_string(char *str, uint8_t size)
{
	uint8_t i = 0;
	uint8_t key_num;
	do
	{
		key_num = rx_byte();
		if(key_num != 13) // If key pressed is different from 'Enter'
		{
			*(str + i) = key_num;
			i++;
		}
	} while (key_num != 13 && i < size);
	
	*(str + i) = '\0';
}

/* Print Number */
void print_number(uint16_t number)
{
	char buffer[5];
	// number, string, base
	itoa(number, buffer, 10);
	put_string(buffer);
}

/* Print Line */
void print_line()
{
	put_string("\n");
}