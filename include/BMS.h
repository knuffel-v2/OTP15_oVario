/**
 * joVario Firmware
 * Copyright (c) 2020 Sebastian Oberschwendtner, sebastian.oberschwendtner@gmail.com
 * Copyright (c) 2020 Jakob Karpfinger, kajacky@gmail.com
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
/**
 ******************************************************************************
 * @file    bms.h
 * @author  SO
 * @version V2.0
 * @date    19-September-2020
 * @brief   Talks to the BMS IC TI BQ25700A via I2C and manages the battery
 * 			charging and the OTG functionality.
 ******************************************************************************
 */

#ifndef BMS_H_
#define BMS_H_
//*********** Includes **************
#include "oVario_Framework.h"


//*********** Defines **************
//Commands for arbiter
#define BMS_CMD_INIT                1 // call-by-reference
#define BMS_CMD_INIT_COULOMB        2 // call-by-reference
#define BMS_CMD_GET_STATUS          3 // call-by-reference
#define BMS_CMD_GET_ADC             4 // call-by-reference
#define BMS_CMD_SET_CHARGE_CURRENT  5 // call-by-value, nargs = 1
#define BMS_CMD_SET_OTG             6 // call-by-value, nargs = 1
#define BMS_CMD_READ_INT_FLASH      7 // call-by-value, nargs = 1
#define BMS_CMD_WRITE_INT_FLASH     8 // call-by-value, nargs = 2
#define BMS_CMD_GET_ADC_COULOMB     9 // call-by-reference
#define BMS_CMD_SAVE_SOC            10// call-by-reference

//Sequences of commands
// INIT
#define BMS_SEQUENCE_SET_OPTION_0   1
#define BMS_SEQUENCE_SET_OPTION_1   2
#define BMS_SEQUENCE_SET_OPTION_2   3
#define BMS_SEQUENCE_SET_OPTION_ADC 4
#define BMS_SEQUENCE_SET_MAX_VOLT   5
#define BMS_SEQUENCE_SET_SYS_VOLT   6
#define BMS_SEQUENCE_SET_INP_CURR   7
// GET_STATUS
#define BMS_SEQUENCE_GET_STATUS     8
// GET_ADC
#define BMS_SEQUENCE_START_ADC      9
#define BMS_SEQUENCE_READ_SYS       10
#define BMS_SEQUENCE_READ_VBUS      11
#define BMS_SEQUENCE_READ_BAT       12
#define BMS_SEQUENCE_READ_INP       13
#define BMS_SEQUENCE_READ_COULOMB   14
// SET_OTG
#define BMS_SEQUENCE_OTG_VOLTAGE    15
#define BMS_SEQUENCE_OTG_CURRENT    16
#define BMS_SEQUENCE_OTG_SET_STATE  17
// READ_INT_FLASH
#define BMS_SEQUENCE_GET_CRC        18
#define BMS_SEQUENCE_GET_LEN        19
// GET_ADC_COULOMB
#define BMS_SEQUENCE_GET_TEMP       20
#define BMS_SEQUENCE_GET_VOLT       21
#define BMS_SEQUENCE_GET_CURRENT    22
#define BMS_SEQUENCE_GET_CHARGE     23
// INIT_COULOMB
#define BMS_SEQUENCE_SET_MAC        24
#define BMS_SEQUENCE_READ_CONFIG_A  25
#define BMS_SEQUENCE_GET_OLD_C      26


/*
 * Register Addresses
 */
//BQ25700
#define CHARGE_OPTION_0_addr		0x12	// R/W
#define CHARGE_OPTION_1_addr		0x30	// R/W
#define CHARGE_OPTION_2_addr		0x31	// R/W
#define CHARGE_OPTION_3_addr		0x32	// R/W
#define CHARGE_CURRENT_addr			0x14	// R/W
#define MAX_CHARGE_VOLTAGE_addr		0x15	// R/W
#define PROCHOT_OPTION_0_addr		0x33	// R/W
#define PROCHOT_OPTION_1_addr		0x34	// R/W
#define ADC_OPTION_addr				0x35	// R/W
#define CHARGER_STATUS_addr			0x20	// R
#define PROCHOT_STATUS_addr			0x21	// R
#define INPUT_LIMIT_DPM_addr		0x22	// R	Used input current limit value
#define ADC_VBUS_addr				0x23	// R	ADC results of VBUS and PSYS
#define ADC_BAT_CURRENT_addr		0x24	// R	ADC	results of battery currents
#define ADC_INPUT_CURRENT_addr		0x25	// R	ADC results of input current and CMPNI
#define ADC_SYS_VOLTAGE_addr		0x26	// R	ADC results of VSYS and VBAT
#define OTG_VOLTAGE_addr			0x3B	// R/W
#define OTG_CURRENT_addr			0x3C	// R/W
#define INPUT_VOLTAGE_addr			0x3D	// R/W
#define MIN_SYS_VOLTAGE_addr		0x3E	// R/W
#define INPUT_LIMIT_HOST_addr		0x3F	// R/W	Set input current limit
#define MANUFACTURER_ID_addr		0xFE	// R/W
#define DEVICE_ID_addr				0xFF	// R/W

//BQ34110
#define CONFIGURATION_A_df_addr		0x413A	// R/W Configration Register, only FLASH access!
#define MAC_STATUS_INIT_df_addr		0x40D7	// R/W Manufacturer status init, only FLASH access!
#define MAC_DF_WRITE_addr			0x4000  // Write Flash data
#define MAC_INFO_BLOCK_addr			0x0070  // Command to acces the MAC Info block, only FLASH access!
#define MAC_addr					0x3E	// R/W Manufacturer Access Control to write commands
#define MAC_DATA_addr				0x40	// R/W
#define MAC_SUM_addr				0x60	// R/W Checksum for commands
#define MAC_LEN_addr				0x61	// R/W Number of bytes included in checksum
#define TEMPERATURE_addr			0x06	// R
#define VOLTAGE_addr				0x08	// R
#define BATTERY_STAT_addr			0x0A	// R
#define CURRENT_addr				0x0C	// R
#define ACC_CHARGE_addr				0x1A	// R

#define RESET_subaddr				0x41

/*
 * Option Bits
 */
//********************BQ25700*************************************************************
//CHARGE_OPTION_0
#define EN_LWPWR					(1<<15)	// Enable low power mode
#define IDPM_AUTO_DISABLE			(1<<12)
#define OTG_ON_CHRGOK				(1<<11)	// ADD OTG to CHRG_OK
#define EN_OOA						(1<<10) // Enable Out-of-Audio
#define PWM_FREQ					(1<<9)	// Set Switching frequency
#define EN_LEARN					(1<<5)
#define IADPT_GAIN					(1<<4)	// IADPT amplifier ratio
#define IBAT_GAIN					(1<<3)	// IBAT amplifier ratio
#define EN_LDO						(1<<2)
#define EN_IDPM						(1<<1)	// Enable IDPM regulation loop
#define CHRG_INHIBIT				(1<<0)

//CHARGE_OPTION_1
#define EN_IBAT						(1<<15)	// Enable IBAT output buffer
#define EN_PSYS						(1<<12)	// Enable PSYS sensing circuit and output buffer
#define RSNS_RAC					(1<<11)	// Set input sense resistor RAC
#define RSNS_RSR					(1<<10)	// set charge sense resistor RSR
#define PSYS_RATIO					(1<<9)	// Ratio of PSYS output current vs. total input battery power
#define CMP_REF						(1<<8)	// Set independent comparator internal reference
#define CMP_POL						(1<<7)	// Set independent comparator polarity
#define FORCE_LATCHOFF				(1<<3)	// Force power path off
#define EN_SHIP_DCHRG				(1<<1)	// Discharge SRN for shipping mode
#define AUTO_WAKEUP_EN				(1<<0)

//CHARGE_OPTION_2
#define EN_PKPWR_IDPM				(1<<13)	// Enable peak power mode triggered by input current overshoot
#define EN_PKPWR_VSYS				(1<<12) // Enable peak power mode triggered by system voltage undershoot
#define PKPWR_OVLD_STAT				(1<<11) // Indicator at the device is in overloading cycle
#define PKPWR_RELAX_STAT			(1<<10) // Indicator at the device is in relaxation cycle
#define EN_EXTILIM					(1<<7)	// Enable ILIM_HIZ pin to set input current limit
#define EN_ICHG_IDCHG				(1<<6) 	// Set function of IBAT pin
#define Q2_OCP						(1<<5)	// Q2 OCP threshold by sensing Q2 VDS
#define ACX_OCP						(1<<4)	// Input current OCP threshold by sensing ACP-ACN
#define EN_ACOC						(1<<3)	// Enable input overcurrent protection by sensing the voltage across ACP-ACN
#define ACOC_VTH					(1<<2)	// Set Mosfet OCP threshold as percentage of IDPM
#define EN_BATOC					(1<<1)	// Enable battery discharge overcurrent protection by sensing the voltage across SRN-SRP
#define BATOC_VTH					(1<<0)	// Set battery discharge overcurrent threshold

//CHARGE_OPTION_3
#define EN_HIZ						(1<<15)	// Device HI-Z mode enable
#define RESET_REG					(1<<14)	// Reset registers
#define RESET_VINDPM				(1<<13)	// Reset VINDPM threshold
#define EN_OTG						(1<<12)	// Enable device OTG mode when EN_OTG pin is HIGH
#define EN_ICO_MODE					(1<<11)	// Enable ICO algorithm
#define BATFETOFF_HIZ				(1<<1)	// Control BATFET during HI-Z mode
#define PSYS_OTG_IDCHG				(1<<0)	// PSYS function during OTG mode

//TODO Add Prochot option bits

//ADC_OPTION
#define ADC_CONV					(1<<15)	// Coonversion mode of ADC
#define ADC_START					(1<<14)	// Start ADC conversion
#define ADC_FULLSCALE				(1<<13)	// ADC input voltage range
#define EN_ADC_CMPIN				(1<<7)
#define EN_ADC_VBUS					(1<<6)
#define EN_ADC_PSYS					(1<<5)
#define EN_ADC_IIN					(1<<4)
#define EN_ADC_IDCHG				(1<<3)
#define EN_ADC_ICHG					(1<<2)
#define EN_ADC_VSYS					(1<<1)
#define EN_ADC_VBAT					(1<<0)

//CHARGER_STATUS
#define AC_STAT						(1<<15)	// Input source status, same as CHRG_OK bit
#define ICO_DONE					(1<<14)	// ICO routine successfully executed
#define IN_VINDPM					(1<<12)
#define IN_IINDPM					(1<<11)
#define IN_FCHRG					(1<<10)	// Fast charge mode
#define	IN_PCHRG					(1<<9)	// Pre-charge mode
#define IN_OTG						(1<<8)	// OTG mode
#define FAULT_ACOV					(1<<7)
#define FAULT_BATOC					(1<<6)
#define FAULT_ACOC					(1<<5)
#define SYSOVP_STAT					(1<<4)	// SYSOVP status and clear
#define FAULT_LATCHOFF				(1<<2)
#define FAULT_OTG_OVP				(1<<1)
#define FAULT_OTG_UCP				(1<<0)

//TODO Add Prochot status register bits


/*
 * Option Bit Positions
 */
//CHARGE_OPTION_0
#define WDTMR_ADJ_pos				13		// Watchdog timer adjust

//CHARGE_OPTION_1
#define EN_PROCHOT_LPWR_pos			13
#define CMP_DEG_pos					4		// Set independent comparator de-glitch time

//CHARGE_OPTION_2
#define PKPWR_TOVL_DEG_pos			14		// Input overload time in peak power mode
#define PKPWR_TMAX_pos				8		// Peak power mode relax and overload cycle time

//Status defines
#define STATUS_INPUT_PRESENT		(1<<15)	//Whether Input power is present
#define STATUS_FAST_CHARGE			(1<<14)
#define STATUS_PRE_CHARGE			(1<<13)
#define STATUS_OTG_EN				(1<<12)	//Whether OTG is enabled
#define STATUS_ADC_REQUESTED		(1<<11)
#define STATUS_ADC_FINISHED			(1<<10)
#define STATUS_CHRG_OK				(1<<9)	//Status of the CHRG_OK pin
#define STATUS_BMS_ACTIVE			(1<<8)  //The BMS sensor is healthy and responding
#define STATUS_GAUGE_ACTIVE         (1<<7)	//The battery gauge (coulomb counter) is healthy and responding
#define STATUS_BAT_PRESENT			(1<<6)  //Whether a battery is present or not, this is detected via the charge current
#define STATUS_INITIALIZED          (1<<5)  //When the task is finished initializing

//********************BQ34110*************************************************************
// CONFIG_A
#define TEMPS						(1<<15)	//Which temperature sensor is used
#define SCALED						(1<<10)	//Scaled Mode in voltage and current
#define SLEEP						(1<<9)	//Wether Sleep mode is possible
#define SLPWAKECHG					(1<<8)
#define JEITA						(1<<7)	//Use temperature profiles for self discharge estimation
#define GNDSEL						(1<<6)	//Select ground reference for adc
#define NIMH_CHG_EN					(1<<5)
#define NI_DT						(1<<4)	//Use dV/dT for charge termination in NiMh mode
#define NI_DV						(1<<3)	//Use -dV for charge termination in NiMh mode
#define IWAKE						(1<<2)
#define	RSNS1						(1<<1)
#define RSNS0						(1<<0)

//MAC_STATUS_INIT
#define WHR_EN						(1<<6)	// WHr charge termination
#define LF_EN						(1<<5)	// Lifetime data collection enable
#define PCTL_EN						(1<<4)	// Pin control enable
#define EOS_EN						(1<<3)	// End-of-Service determination enable
#define IGNORE_SD_EN				(1<<2)	// Ignore self discharge control
#define ACCHG_EN					(1<<1)	// Accumulated charge enable for charging current integration
#define ACDSG_EN					(1<<0)	// Accumulated charge enable for discharging current integration

// ***** Functions *****
void            bms_register_ipc        (void);
void            bms_task                (void);
void            bms_check_semaphores    (void);
void            bms_idle                (void);
void            bms_init                (void);
void            coulomb_init            (void);
void            bms_get_status          (void);
void            bms_get_adc             (void);
void            bms_set_charge_current  (void);
void            bms_set_otg             (void);
void            coulomb_read_int_flash  (void);
void            coulomb_write_int_flash (void);
void            coulomb_get_adc         (void);
void            coulomb_save_soc        (void);
void            bms_init_peripherals    (void);
void            bms_call_task           (unsigned char cmd, unsigned long data, unsigned char did_target);
void            coulomb_call_task       (unsigned char cmd, unsigned long data, unsigned char did_target);
#endif /* BMS_H_ */
