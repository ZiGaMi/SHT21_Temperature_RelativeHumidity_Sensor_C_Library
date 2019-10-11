/*
 * SHT21.c
 *
 *  Created on: 11. okt. 2019
 *      Author: ZigaM
 */

#include "Drivers/SHT21.h"




// Raw temperature data
uint16_t g_rawTempData;

// Temperature
float gf_temperature;

// Raw RH
uint16_t g_rawRHData;

// HV
float gf_HV;






// Init SHT21
void SHT21_Init(){

	// Init I2C
	I2c_Init();

	// Wait for power-up sequence
	delay_ms(15);

	// CONFIGURE DEVICE - TODO:


}


// Get raw temperature data
uint16_t SHT21_GetRawTemp()
{
	return g_rawTempData;
}


// Get temperature
float SHT21_GetTemp()
{
	return gf_temperature;
}


// Get raw RH data
uint16_t SHT21_GetRawRH()
{
	return g_rawRHData;
}


// Get RH
float SHT21_GetRH()
{
	return gf_HV;
}


// Read temperature
uint8_t SHT21_ReadTemp()
{
	uint8_t nack = 1;
	uint8_t attempt = 0;
	uint8_t *data;

	// Trigger TEMP measurement
	while (( nack == 1 ) && ( attempt < 10 ))
	{
		nack = I2c_Write(SHT21_ADDR, SHT21_TRIG_TEMP_MEAS_CMD, 1u);
		attempt++;
		delay_ms(5);
	}


	// Read TEMP
	data = I2c_Read(SHT21_ADDR, 3u);
	g_rawTempData = ((( *data << 8u ) & 0xFF00u ) | ( *( data + 1u ) & 0xFCu ));

	// Convert
	gf_temperature = ( float ) ( -46.85 + ( 175.72f * g_rawTempData / 65536.0f ));

	// Check CRC
	// TODO: Implement CRC calculator


	// Based on CRC
	return 1;
}


// Read RH
uint8_t SHT21_ReadRH()
{
	uint8_t nack = 1;
	uint8_t attempt = 0;
	uint8_t *data;

	// Trigger TEMP measurement
	while (( nack == 1 ) && ( attempt < 10 ))
	{
		nack = I2c_Write(SHT21_ADDR, SHT21_TRIG_RH_MEAS_CMD, 1u);
		attempt++;
		delay_ms(5);
	}


	// Read HV
	data = I2c_Read(SHT21_ADDR, 3u);
	g_rawRHData = ((( *data << 8u ) & 0xFF00u ) | ( *( data + 1u ) & 0xF0u ));

	// Convert
	gf_HV = ( float ) ( -6.0f + ( 125.0f * g_rawRHData / 65536.0f ));

	// Check CRC
	// TODO: Implement CRC calculator


	// Based on CRC
	return 1;
}
















