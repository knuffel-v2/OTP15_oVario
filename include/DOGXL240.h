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
 * @file    DOGXL240.h
 * @author  SO
 * @version V1.0
 * @date    28-February-2018
 * @brief   Talks to the DOGXL display via the SPI bus.
 ******************************************************************************
 */

#ifndef DOGXL240_H_
#define DOGXL240_H_

#include "oVario_Framework.h"


/*
 * Define for visualisation
 */
//Define amount of pixels
#define LCD_PIXEL_X		240
#define LCD_PIXEL_Y		128
//Define font size
#define FONT_X			6
#define FONT_Y			8
/*
 * Defines for registers and commands
 */
#define COMMAND				0
#define DATA				1

/*
 * Control registers
 */
#define SET_COL_LSB			0b00000000
#define SET_COL_MSB			0b00010000
#define SET_PAGE_LSB		0b01100000
#define SET_PAGE_MSB		0b01110000
#define SET_TEMP_COMP		0b00100100
#define SET_PAN_LOAD		0b00101000
#define SET_POTI			0b10000001
#define SET_DISP_EN			0b10101000
#define SET_LCD_MAP			0b11000000
#define SET_LCD_BIAS		0b11101000
#define SET_COM_END			0b11110001
#define SET_PART_DISP_START	0b11110010
#define SET_PART_DISP_END	0b11110011
#define SET_ALL_ON			0b10100100
#define SET_LINE_RATE		0b10100000
#define SET_DISP_PAT		0b11010000
#define SET_RAM_ADDR_CTRL	0b10001000
#define SYS_RESET			0b11100010
#define SET_DISP_INV		0b10100110
#define SET_WPC0			0b11110100
#define SET_WPP0			0b11110101
#define SET_WPC1			0b11110110
#define SET_WPP1			0b11110111
#define SET_WPP_EN			0b11111000

/*
 * Control bits
 */
//In SET_ALL_ON
#define ALL_ON_DC1			0b00000001
//In SET_DISP_EN
#define DISP_EN_DC2			0b00000001
#define DISP_EN_DC3			0b00000010
#define DISP_EN_DC4			0b00000100

//In SET_TEMP_COMP
#define TEMP_COMP_005		0b00000000
#define TEMP_COMP_010		0b00000001
#define TEMP_COMP_015		0b00000010
#define TEMP_COMP_000		0b00000011

//In SET_LINE_RATE (only applicable in b/w-mode)
#define LINE_RATE_5_9KLPS	0b00
#define LINE_RATE_7_1KLPS	0b01
#define LINE_RATE_8_2KLPS	0b10
#define LINE_RATE_9_4KLPS	0b11

//In SET_DISP_PAT
#define DISP_PAT_DC5		0b001
#define DISP_PAT_DC6		0b010
#define DISP_PAT_DC7		0b100

#define DISP_PAT_PATTERN0	0b000
#define DISP_PAT_PATTERN1	DISP_PAT_DC6
#define DISP_PAT_PATTERN2	DISP_PAT_DC7
#define DISP_PAT_PATTERN3	DISP_PAT_DC6 | DISP_PAT_DC7

//In SET_COL_LSB
#define COL_LSB_CA0			0b0001
#define COL_LSB_CA1			0b0010
#define COL_LSB_CA2			0b0100
#define COL_LSB_CA3			0b1000

//In SET_COL_MSB
#define COL_MSB_CA4			0b0001
#define COL_MSB_CA5			0b0010
#define COL_MSB_CA6			0b0100
#define COL_MSB_CA7			0b1000

//In SET_PAGE_LSB
#define COL_LSB_PA0			0b0001
#define COL_LSB_PA1			0b0010
#define COL_LSB_PA2			0b0100
#define COL_LSB_PA3			0b1000

//In SET_PAGE_MSB
#define PAGE_MSB_PA4		0b0001
#define PAGE_MSB_PA5		0b0010
#define PAGE_MSB_PA6		0b0100

#define PAGE_PATTERN0		0
#define PAGE_PATTERN1		PAGE_MSB_PA5
#define PAGE_PATTERN2		PAGE_MSB_PA6
#define PAGE_PATTERN3		PAGE_MSB_PA5 | COL_MSB_PA6

//In SET_RAM_ADDR_CTRL
#define RAM_ADDR_CTRL_AC0	0b001
#define RAM_ADDR_CTRL_AC1	0b010
#define RAM_ADDR_CTRL_AC2	0b100

//In SET_LCD_BIAS
#define LCD_BIAS_5			0b00
#define LCD_BIAS_10			0b01
#define LCD_BIAS_11			0b10
#define LCD_BIAS_12			0b11

//In SET_DISP_INV
#define DISP_INV_DC0		1

//In SET_LCD_MAP
#define LCD_MAP_MX			(1<<1)
#define LCD_MAP_MY			(1<<2)

void init_lcd               (void);
void lcd_dma_enable         (void);
void lcd_reset              (void);
void lcd_set_cd             (unsigned char ch_state);
void lcd_set_col_addr       (unsigned char ch_col);
void lcd_set_page_addr      (unsigned char ch_page);
void lcd_set_write_pattern  (unsigned char ch_pat);
void lcd_set_pattern        (unsigned char ch_pat);
void lcd_set_enable         (unsigned char ch_state);
void lcd_set_cursor         (unsigned char ch_x, unsigned char ch_y);
void lcd_shift_cursor       (signed char ch_pos_x, signed char ch_pos_y);
void lcd_set_fontsize       (unsigned char ch_size);
void lcd_set_inverted       (unsigned char ch_state);
void lcd_send_buffer        (void);
void lcd_pixel2buffer       (unsigned char ch_x, unsigned char ch_y, unsigned char ch_val);
void lcd_char2buffer        (unsigned char ch_data);
void lcd_clear_buffer       (void);
void lcd_string2buffer      (char* pch_string);
void lcd_num2buffer         (unsigned long l_number,unsigned char ch_predecimal);
void lcd_signed_num2buffer  (signed long l_number,unsigned char ch_predecimal);
void lcd_digit2buffer       (unsigned char ch_data);
void lcd_float2buffer       (float f_number, unsigned char ch_predecimal, unsigned char ch_dedecimal);
void lcd_bat2buffer         (unsigned char ch_stat);
void lcd_line2buffer        (unsigned char ch_x_start,unsigned char ch_y_start,unsigned char ch_x_end,unsigned char ch_y_end);
void lcd_circle2buffer      (unsigned char ch_x_center, unsigned char ch_y_center, unsigned char ch_radius);
void lcd_block2buffer       (unsigned char ch_x, unsigned char ch_y, unsigned char ch_height, unsigned char ch_width);
void lcd_box2buffer         (unsigned char width, unsigned char height, unsigned char thickness);
void lcd_arrow2buffer       (unsigned char ch_x_center, unsigned char ch_y_center, unsigned char ch_radius, uint16_t heading);

#endif /* DOGXL240_H_ */
