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
//		SHT21 specifics
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
#define SHT21_READ_USR_REG_CMD				( uint8_t ) ( 0xFEu )




////////////////////////////////////////////////////////////////////////////////
//
//		Function Prototypes
//
////////////////////////////////////////////////////////////////////////////////

void SHT21_Init();
uint16_t SHT21_GetRawTemp(void);
float SHT21_GetTemp(void);
uint16_t SHT21_GetRawRH(void);
float SHT21_GetRH(void);
uint8_t SHT21_ReadTemp(void);
uint8_t SHT21_ReadRH(void);






#endif /* DRIVERS_SHT21_H_ */
