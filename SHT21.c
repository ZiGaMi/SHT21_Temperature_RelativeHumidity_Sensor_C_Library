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

	// Reset device
	SHT21_SoftReset();

	// Disable heater
	SHT21_HeaterEnable(0);
}


// Reset sensor
void SHT21_SoftReset(){

	// SF-RST command
	I2c_Write(SHT21_ADDR, SHT21_SOFT_RESET_CMD, NULL, 1);

	// Wait for recover
	delay_ms(15);
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


// Write command
uint8_t SHT21_WriteCommand(uint8_t cmd)
{
	uint8_t nack = 1;
	uint8_t attempt = 0;

	// Try until ack or attempt overrun
	while (( nack == 1 ) && ( attempt < SHT21_ATTEMPT_NUM ))
	{

		// Write command to device
		nack = I2c_Write(SHT21_ADDR, cmd, NULL, 1);

		// New attempt
		attempt++;

		// Wait
		delay_ms(10);
	}

	// Check if device response to command
	if ( SHT21_ATTEMPT_NUM == attempt )
	{
		return 0;	// Error - cannot write to device
	}
	else
	{
		return 1;	// No error - device respond
	}
}


// Read temperature
// NOTE: Reading temperature under 1s is not recommended, as sensor self heating
// effect the measurements of temperature and RH.
uint8_t SHT21_ReadTemp()
{
	uint8_t ack;
	uint8_t *data;
	uint8_t crc_ok = 0;

	// Temperature measurement
	ack = SHT21_WriteCommand(SHT21_TRIG_TEMP_MEAS_CMD);

	// Check device respons
	if ( 1 == ack )
	{
		// Read TEMP
		data = I2c_Read(SHT21_ADDR, 3u);
		g_rawTempData = ((( *data << 8u ) & 0xFF00u ) | ( *( data + 1u ) & 0xFCu ));

		// Convert
		gf_temperature = ( float ) ( -46.85 + ( 175.72f * g_rawTempData / 65536.0f ));

		// Check CRC
		crc_ok = SHT21_CheckCRC(data, 2u, *( data + 2u ));

		// Based on CRC
		return crc_ok;
	}
	else
	{
		return 0;	// Device not responded
	}

}



// Read RH
uint8_t SHT21_ReadRH()
{
	uint8_t ack;
	uint8_t *data;
	uint8_t crc_ok = 0;

	// Relative humidity measurements
	ack = SHT21_WriteCommand(SHT21_TRIG_RH_MEAS_CMD);

	// Check ack
	if ( 1u == ack )
	{
		// Read HV
		data = I2c_Read(SHT21_ADDR, 3u);
		g_rawRHData = ((( *data << 8u ) & 0xFF00u ) | ( *( data + 1u ) & 0xF0u ));

		// Convert
		gf_HV = ( float ) ( -6.0f + ( 125.0f * g_rawRHData / 65536.0f ));

		// Check CRC
		crc_ok = SHT21_CheckCRC(data, 2u, *( data + 2u ));

		// Based on CRC
		return crc_ok;
	}
	else
	{
		return 0; // Device not responded
	}
}



// Check CRC sum
// NOTE: This code is taken from SHT21 sample code
uint8_t SHT21_CheckCRC(uint8_t *data, uint8_t size, uint8_t crc)
{

	uint8_t crc_calc = 0;
	uint8_t i;
	uint8_t bit;

	//calculates 8-Bit checksum with given polynomial
	for (i = 0; i < size; i++)
	{
		crc_calc ^= ( data[i] );

		for ( bit = 8; bit > 0; --bit)
		{
			if (crc_calc & 0x80)
			{
				crc_calc = (( crc_calc << 1u ) ^ SHT21_CRC_POLY );
			}
			else
			{
				crc_calc = ( crc_calc << 1u );
			}
		}
	}

	if ( crc_calc == crc )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


// Read from user register
uint8_t SHT21_ReadUserRegister()
{
	uint8_t *reg;
	uint8_t ack;

	// Read from user register
	ack = SHT21_WriteCommand(SHT21_READ_USR_REG_CMD);

	// Check ack
	if ( 1u == ack )
	{
		// Get value from user register
		reg = I2c_Read(SHT21_ADDR, 1u);

		// Return readed value
		return (uint8_t) ( *reg );
	}
	else
	{
		return 0;
	}
}


// Write to user register
void SHT21_WriteUserRegister(uint8_t reg)
{
	// Write to reg
	I2c_Write(SHT21_ADDR, SHT21_WRITE_USR_REG_CMD, &reg, 2u);

}


// Perform self-test
uint8_t SHT21_SelfTestProcedure()
{


	// Enable heater
	SHT21_HeaterEnable(1);


	// Perform test....
	// TODO:


	// Disable heater
	SHT21_HeaterEnable(0);

	return 0;
}




// Start mesurements
uint8_t SHT21_StartMeasurements()
{
	uint8_t err = 0;

	// Read temperature from sensor - blocking
	err = SHT21_ReadTemp();

	// Continue with measurements
	if ( 1u == err )
	{
		// Wait
		delay_ms(10);

		// Read RH from sensor - blocking
		err = SHT21_ReadRH();

		// Everythink OK
		return 1;
	}

	// Stop with measurements
	else
	{
		return 0; // Measurement fails
	}
}


// Enable/disable heater
void SHT21_HeaterEnable(uint8_t en)
{
	uint8_t user_reg = SHT21_ReadUserRegister();

	if ( 1u == en )
	{
		user_reg |= ( SHT21_USR_REG_EN_HEATER_msk );
	}
	else
	{
		user_reg &= ~( SHT21_USR_REG_EN_HEATER_msk );
	}

	// Write changes to user register
	SHT21_WriteUserRegister(user_reg);
}










