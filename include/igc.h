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

//*********** Defines **************
#define IGC_MANUF_ID		"XCS"		//The igc code has to mimic the XCSOAR header!
#define IGC_LOGGER_ID		"AAA"		//The unique id of the logger for the A-record
#define IGC_PILOT_NAME		"Sebastian Oberschwendtner"
#define IGC_GLIDER_TYPE		"Paraglider"
#define IGC_GLIDER_ID		"none"
#define IGC_FIRMWARE_VER	"V1.0"
#define IGC_HARDWARE_VER	"REVA"
#define IGC_FR_TYPE			"XCSOAR,XCSOAR Android 6.5 Mar  8 2013"	//Important to mimic XCSOAR header!
#define IGC_GPS_RX			"uBlox, NEO-6M,16,max9000m"				//TODO Add correct GPS information (MaxAlt + Channels)
#define IGC_BARO_MANUF		"MS5611,max10000m"						//TODO Add correct Baro information (MaxAlt)

//G-Record
#define IGC_HASH_NUMBER			4
#define IGC_DIGEST_CHARPERLINE	16

//States for igc file
#define IGC_CLOSED			0
#define IGC_RECORDING		1
#define IGC_LANDING			2
#define IGC_FINISHED		3
#define IGC_ERROR			255

//*********** Functions **************

#endif /* IGC_H_ */
