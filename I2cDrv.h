/*
 * I2cDrv.h
 *
 *  Created on: 4. okt. 2019
 *      Author: ZigaM
 */

#ifndef DRIVERS_I2CDRV_H_
#define DRIVERS_I2CDRV_H_


#include "stm32f0xx.h"
#include "stdlib.h"



////////////////////////////////////////////////////////////////////////////////
//
//		Port / Pins
//
////////////////////////////////////////////////////////////////////////////////

#define I2C_port 		( GPIOA )

#define I2C_SCL_bp		( 9ul )
#define I2C_SCL_msk		( 0x01u << I2C_SCL_bp )
#define I2C_SCL_afnum	( 4ul )

#define I2C_SDA_bp		( 10ul )
#define I2C_SDA_msk		( 0x01u << I2C_SDA_bp )
#define I2C_SDA_afnum	( 4ul )




////////////////////////////////////////////////////////////////////////////////
//
//		Pheripery
//
////////////////////////////////////////////////////////////////////////////////

#define USER_I2C		( I2C1 )






////////////////////////////////////////////////////////////////////////////////
//
//		Function Prototypes
//
////////////////////////////////////////////////////////////////////////////////

// Init pins
void I2c_Pins_Init(void);

// Init I2C
void I2c_Init(void);

// Write to device
uint8_t I2c_Write(uint8_t addr, uint8_t cmd, uint8_t *data, uint8_t n_byte);

// Read from device
uint8_t* I2c_Read(uint8_t addr, uint8_t n_byte);


#endif /* DRIVERS_I2CDRV_H_ */
