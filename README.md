# SHT21_C_Library
Complete C library for temperature/relative humidity sensor SHT21 based on STM32F031K6 microprocessor. 

## Example

		// Read temperature from sensor - blocking
		SHT21_ReadTemp();

		// Get converted value
		f_T = SHT21_GetTemp();

		// Wait
		delay_ms(10);

		// Read RH from sensor - blocking
		SHT21_ReadRH();

		// Get converted value
		f_RH = SHT21_GetRH();
