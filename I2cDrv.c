/*
 * I2cDrv.c
 *
 *  Created on: 4. okt. 2019
 *      Author: ZigaM
 */


#include "Drivers/I2cDrv.h"

// Init pins
void I2c_Pins_Init(){

	// Enable clock
	RCC -> AHBENR |= ( RCC_AHBENR_GPIOAEN );

	// Alternative function
	I2C_port -> MODER &= ~(	( 0x03u  << ( 2ul * I2C_SCL_bp ))	|
							( 0x03u  << ( 2ul * I2C_SDA_bp ))	);
	I2C_port -> MODER |=  (	( 0x02u  << ( 2ul * I2C_SCL_bp ))	|
							( 0x02u  << ( 2ul * I2C_SDA_bp ))	);

	// Alternative function number
	I2C_port -> AFR[1] &= ~(( 0x0Fu << ( 4ul * ( I2C_SCL_bp - 8ul ))) 	|
							( 0x0Fu << ( 4ul * ( I2C_SDA_bp - 8ul )))	);
	I2C_port -> AFR[1] |= (( I2C_SCL_afnum << ( 4ul * ( I2C_SCL_bp - 8ul ))) 	|
							( I2C_SDA_afnum << ( 4ul * ( I2C_SDA_bp - 8ul )))	);
}



// Init I2C
void I2c_Init(){

	// Init pins
	I2c_Pins_Init();

	// Set system clock as I2C clock
	RCC -> CFGR3 |= ( RCC_CFGR3_I2C1SW_SYSCLK );

	// Enable I2C clock
	RCC -> APB1ENR |= ( RCC_APB1ENR_I2C1EN );

	// Timing calculations according to
	USER_I2C -> TIMINGR = (uint32_t) ( 0x10805E89 );

	// Enable
	USER_I2C -> CR1 = I2C_CR1_PE;

	// Enable auto generation of STOP when nbytes are sended
	USER_I2C -> CR2 = ( I2C_CR2_AUTOEND );

}


// Write to device
uint8_t I2c_Write(uint8_t addr, uint8_t cmd, uint8_t *data, uint8_t n_byte){

	uint8_t i = 0;

	// Write transfer
	USER_I2C -> CR2 &= ~( I2C_CR2_RD_WRN | I2C_CR2_NBYTES_Msk );

	// Address (7-bit)
	USER_I2C -> CR2 |= ((( addr << 1u ) & 0xFEu ) | (( n_byte << I2C_CR2_NBYTES_Pos ) & I2C_CR2_NBYTES_Msk ));

	// Data to send
	USER_I2C -> TXDR = ( uint8_t ) ( cmd & 0xFFu );

	// Send
	USER_I2C -> CR2 |= ( I2C_CR2_START );

	// Check if any of data to transmit at all
	if ( data != NULL ){
		for( i = 0; i < ( n_byte - 1u ); i++ )
		{
			while(( USER_I2C -> ISR & I2C_ISR_TXE ) != I2C_ISR_TXE );
			USER_I2C -> TXDR = ( uint8_t ) ( data[i] & 0xFFu );
		}
	}

	// Check ack
	if (( USER_I2C -> ISR & I2C_ISR_NACKF ) == I2C_ISR_NACKF )
	{
		return 1;			// No ack
	}
	else
	{
		return 0;			// Ack
	}
}


// Read from device
uint8_t* I2c_Read(uint8_t addr, uint8_t n_byte){

	uint8_t nack;
	static uint8_t data[10];

	// Clear number of bytes
	USER_I2C -> CR2 &= ~( I2C_CR2_NBYTES_Msk );

	// Address (7-bit)
	USER_I2C -> CR2 |= ( uint32_t ) ((( addr << 1u ) & 0xFEu ) | ( I2C_CR2_RD_WRN ) | (( n_byte << I2C_CR2_NBYTES_Pos ) & I2C_CR2_NBYTES_Msk ));

	// Send
	USER_I2C -> CR2 |= ( uint32_t ) ( I2C_CR2_START );

	// Get data
	uint8_t i;
	for ( i = 0; i < n_byte; i++ )
	{
		while (( USER_I2C -> ISR & I2C_ISR_RXNE ) != I2C_ISR_RXNE );
		data[i] = ( USER_I2C -> RXDR );
	}


	// Check for nack
	nack = (( USER_I2C -> ISR & I2C_ISR_NACKF ) == I2C_ISR_NACKF );

	if ( 1 == nack )
	{
		return NULL;
	}
	else
	{
		return ( uint8_t* ) ( &data );
	}
}

