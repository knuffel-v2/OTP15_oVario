/*
 * ms5611.c
 *
 *  Created on: 16.03.2018
 *      Author: Admin
 */

#ifndef MS5611_C_
#define MS5611_C_

#include "MS5611.h"
#include "Variables.h"

// ***** Variables *****
uint32_t C0,C1,C2,C3,C4,C5,C6,CRC_MS,D1,D2;
int32_t dT, Temp;
uint64_t T2;
uint64_t Off2;
uint64_t Sens2;
int32_t calib_pressure = 0;
int32_t ipc_pressure = 1;

datafusion_T msdata;




// ***** Functions *****

void MS5611_init()
{
	// Perform Reset
	wait_systick(5);
	i2c_send_char(i2c_addr_MS5611, 0x1E);

	wait_systick(2);


	// PROM Read
	C0 =		i2c_read_int(i2c_addr_MS5611,0xA0);							// Load constants for calculation
	C1 =		i2c_read_int(i2c_addr_MS5611,0xA2);
	C2 =		i2c_read_int(i2c_addr_MS5611,0xA4);
	C3 =		i2c_read_int(i2c_addr_MS5611,0xA6);
	C4 =		i2c_read_int(i2c_addr_MS5611,0xA8);
	C5 =		i2c_read_int(i2c_addr_MS5611,0xAA);
	C6 =		i2c_read_int(i2c_addr_MS5611,0xAC);
	CRC_MS =	i2c_read_int(i2c_addr_MS5611,0xAE);

	wait_systick(1);
	wait_ms(100ul);


	ipc_memory_register(44,did_MS5611); //todo auf realen IPC Datentyp anpassen
}

// function to read & calculate pressure
int32_t get_pressure_MS()
{
	catch_temp_MS();
	// Convert D1 OSR = 4096
	i2c_send_char(i2c_addr_MS5611, 0x48);

	wait_ms(10ul);

	// ADC Read
	uint32_t D1 = (int32_t)i2c_read_24bit(i2c_addr_MS5611,0x00);

	// ADC Read

	int64_t Off = (long long)C2 * 65536 + ((long long)C4 * dT ) / 128;
	int64_t Sens = (long long)C1 * 32768 + ((long long)C3 * dT) / 256;

	if (Temp < 2000)
	{
		uint64_t T2 = ((uint64_t) dT )>> 31;
		uint64_t Off2 = (5 * (Temp - 2000)^2) >> 1;
		uint64_t Sens2 = (5 * (Temp - 2000)) >> 2;

		if (Temp < -15)
		{
			Off2 = Off2 + (7 * (Temp + 1500)^2);
			Sens2 = Sens2 + ((11 * (Temp + 1500)^2) >> 1);
		}
	}
	else
	{
		uint64_t T2 = 0;
		uint64_t Off2 = 0;
		uint64_t Sens2 = 0;
	}
	Temp 	= Temp - T2;
	Off 	= Off - Off2;
	Sens 	= Sens - Sens2;

	int32_t pressure 	= (D1*Sens/2097152 - Off)/32768;
	return pressure;
}


// function to read temperature
void catch_temp_MS()
{
	// Convert D2 OSR = 4096
	i2c_send_char(i2c_addr_MS5611, 0x58);
	wait_ms(10ul);

	// Read ADC
	D2 = (int32_t)i2c_read_24bit(i2c_addr_MS5611,0x00);

	dT = D2-((long)C5<<8);
	Temp = 2000 + ((long long)dT * C6)/8388608;
}


// function to read temperature
int32_t get_temp_MS()
{
	catch_temp_MS();
	int32_t Temp_temp = Temp;
	return Temp_temp;
}
#endif /* MS5611_C_ */