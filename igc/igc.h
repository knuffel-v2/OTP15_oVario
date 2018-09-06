/*
 * igc.h
 *
 *  Created on: 02.09.2018
 *      Author: Sebastian
 */

#ifndef IGC_H_
#define IGC_H_
//*********** Includes **************
#include "oVario_Framework.h"
#include "Variables.h"
#include "sdio.h"
#include "ipc.h"
#include "did.h"
#include "md5.h"

//*********** Defines **************
#define IGC_MANUF_ID		"XCS"		//The igc code has to mimic the XCSOAR header!
#define IGC_LOGGER_ID		"AAA"		//The unique id of the logger for the A-record
#define IGC_PILOT_NAME		"He du!"
#define IGC_GLIDER_TYPE		"Paraglider"
#define IGC_GLIDER_ID		"VQUADRAT"
#define IGC_FIRMWARE_VER	"V1.0"
#define IGC_HARDWARE_VER	"REVA"
#define IGC_FR_TYPE			"XCSOAR,XCSOAR Android 6.5 Mar  8 2013"	//Important to mimic XCSOAR header!
#define IGC_GPS_RX			"uBlox, NEO-6M,16,max9000m"				//TODO Add correct GPS information (MaxAlt + Channels)
#define IGC_BARO_MANUF		"MS5611,max10000m"						//TODO Add correct Baro information (MaxAlt)

//*********** Functions **************
unsigned char igc_IsValidCharacter(unsigned char character);
unsigned char igc_IncludeInGrecord(char* in);
void igc_create(void);
void igc_CommitCharacter(unsigned char character);
void igc_CommitLine(char* line);
void igc_WriteLine(void);
void igc_NewRecord(unsigned char type);
void igc_AppendString(char* string);
void igc_AppendNumber(unsigned long number, unsigned char digits);

#endif /* IGC_H_ */