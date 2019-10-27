/*
 * SHT.h
 *
 *  Created on: 11. okt. 2019
 *      Author: ZigaM
 */

#ifndef DRIVERS_SHT21_H_
#define DRIVERS_SHT21_H_

#include "Drivers/I2cDrv.h"
#include "Drivers/ClockDrv.h"




////////////////////////////////////////////////////////////////////////////////
//
//		SHT21 Commands
//
////////////////////////////////////////////////////////////////////////////////

// Device address
#define SHT21_ADDR							( uint8_t ) ( 0x40u )

// Trigger temperature measurement
#define SHT21_TRIG_TEMP_MEAS_CMD			( uint8_t ) ( 0xE3u )

//Trigger relative humidity measurement
#define SHT21_TRIG_RH_MEAS_CMD				( uint8_t ) ( 0xE5u )

// Write user register
#define SHT21_WRITE_USR_REG_CMD				( uint8_t ) ( 0xE6u )

// Read user register
#define SHT21_READ_USR_REG_CMD				( uint8_t ) ( 0xE7u )

// Perform soft reset
#define SHT21_SOFT_RESET_CMD				( uint8_t ) ( 0xFEu )

// Number of attempts to write to device
#define SHT21_ATTEMPT_NUM					( uint8_t ) ( 3u )


////////////////////////////////////////////////////////////////////////////////
//
//		SHT21 CRC Polynomial
//
////////////////////////////////////////////////////////////////////////////////

// CRC polynomial
#define SHT21_CRC_POLY						( uint16_t ) ( 0x131u )




////////////////////////////////////////////////////////////////////////////////
//
//		SHT21 User Register Bit Description
//
////////////////////////////////////////////////////////////////////////////////

// Measurement resolution
#define SHT21_USR_REG_RESOLUTION_1_bp		( uint8_t ) ( 7u )
#define SHT21_USR_REG_RESOLUTION_1_msk		( uint8_t ) ( 0x01u << SHT21_USR_REG_RESOLUTION_1_bp )
#define SHT21_USR_REG_RESOLUTION_0_bp		( uint8_t ) ( 0u )
#define SHT21_USR_REG_RESOLUTION_0_msk		( uint8_t ) ( 0x01u << SHT21_USR_REG_RESOLUTION_0_bp )

// End of battery
#define SHT21_USR_REG_END_OF_BATTERY_bp		( uint8_t ) ( 6u )
#define SHT21_USR_REG_END_OF_BATTERY_msk	( uint8_t ) ( 0x01u << SHT21_USR_REG_END_OF_BATTERY_bp )

// On chip heater enable/disable
#define SHT21_USR_REG_EN_HEATER_bp			( uint8_t ) ( 2u )
#define SHT21_USR_REG_EN_HEATER_msk			( uint8_t ) ( 0x01u << SHT21_USR_REG_EN_HEATER_bp )

// OTP Raload enable/disable
#define SHT21_USR_REG_EN_OPT_RELOAD_bp		( uint8_t ) ( 1u )
#define SHT21_USR_REG_EN_OPT_RELOAD_msk		( uint8_t ) ( 0x01u << SHT21_USR_REG_EN_OPT_RELOAD_bp )





////////////////////////////////////////////////////////////////////////////////
//
//		Function Prototypes
//
////////////////////////////////////////////////////////////////////////////////

// Init sensor
void SHT21_Init(void);

// Reset sensor
void SHT21_SoftReset(void);

// Get raw temperature
uint16_t SHT21_GetRawTemp(void);

// Get converted temperature
float SHT21_GetTemp(void);

// Get raw RH
uint16_t SHT21_GetRawRH(void);

// Get converted RH
float SHT21_GetRH(void);

// Write command
uint8_t SHT21_WriteCommand(uint8_t cmd);

// Trigger temp measurement and read it
uint8_t SHT21_ReadTemp(void);

// Trigger HV measurement and read it
uint8_t SHT21_ReadRH(void);

// Check CRC sum
uint8_t SHT21_CheckCRC(uint8_t *data, uint8_t size, uint8_t crc);

// Read from user register
uint8_t SHT21_ReadUserRegister(void);

// Write to user register
void SHT21_WriteUserRegister(uint8_t reg);

// Perform self-test
uint8_t SHT21_SelfTestProcedure(void);

// Start mesurements
uint8_t SHT21_StartMeasurements(void);

// Enable/disable heater
void SHT21_HeaterEnable(uint8_t en);


#endif /* DRIVERS_SHT21_H_ */
