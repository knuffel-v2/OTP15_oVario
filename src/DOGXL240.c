/*
 * DOGXL240.c
 *
 *  Created on: 28.02.2018
 *      Author: Sebastian
 */

#include "DOGXL240.h"
#include "font.h"

typedef struct
{
	unsigned char cursor_x;
	unsigned char cursor_y;
	unsigned char buffer[LCD_PIXEL_X*LCD_PIXEL_Y/8];
} lcd;

lcd* plcd_DOGXL = 0;

extern const unsigned char font[256][12];

/*
 * Initialize necessary peripherals and display
 */
void init_lcd(void)
{
	init_spi();

	/*Set GPIO [Clock already activated in spi_init()]
	 * PB14:	OUT   (C/D)
	 */
	GPIOB->MODER |= GPIO_MODER_MODER14_0;

	/*
	 * Initialize display
	 */
	lcd_set_cd(COMMAND);

	//Set NSS low at the beginning, display stays selected the whole time
	GPIOB->BSRRH = GPIO_BSRR_BS_12;
	wait_ms(150);
	spi_send_char(SYS_RESET);
	wait_ms(5);

	spi_send_char(SET_COM_END);						//Set com end to 127
	spi_send_char(127);
	spi_send_char(SET_PART_DISP_START);				//Set partial display start to 0
	spi_send_char(0);
	spi_send_char(SET_PART_DISP_END);				//Set partial display start to 127
	spi_send_char(127);
	spi_send_char(SET_TEMP_COMP | TEMP_COMP_010);	//Set Temp comp to -0.10 % per C
	spi_send_char(SET_LCD_MAP);						//LCD Mapping
	spi_send_char(0);
	spi_send_char(SET_LCD_BIAS | LCD_BIAS_11);		//Set bias ratio to 11
	spi_send_char(SET_POTI);						//Set Contrast to 143
	spi_send_char(143);
	spi_send_char(SET_DISP_EN | DISP_EN_DC2);		//Display enable
	spi_send_char(SET_DISP_PAT | DISP_PAT_DC5);		//Set 1bit per pixel in RAM
	spi_send_char(SET_RAM_ADDR_CTRL | RAM_ADDR_CTRL_AC1 | RAM_ADDR_CTRL_AC0); //Automatic wrap around in RAM
	spi_send_char(SET_WPC0);						//Set window programm starting column address
	spi_send_char(0);
	spi_send_char(SET_WPP0);						//Set window programm starting page address
	spi_send_char(0);
	spi_send_char(SET_WPC1);						//Set window programm end column address
	spi_send_char(239);
	spi_send_char(SET_WPP1);						//Set window programm end page address
	spi_send_char(15);
	spi_send_char(SET_WPP_EN);						//Enable window programming
	//spi_send_char(SET_ALL_ON | ALL_ON_DC1);			//All pixel on

	//Set column, page and pattern which sould be written
	lcd_set_page_addr(0);
	lcd_set_col_addr(0);
	lcd_set_write_pattern(PAGE_PATTERN0);

	//register buffer
	plcd_DOGXL = ipc_memory_register(LCD_PIXEL_X*LCD_PIXEL_Y/8,did_LCD);

	lcd_set_cursor(0,0);
}
/*
 * Reset display
 */
void lcd_reset(void)
{
	lcd_set_cd(COMMAND);
	spi_send_char(SYS_RESET);
	wait_ms(5);
}
/*
 * Set C/D pin according to send data
 */
void lcd_set_cd(unsigned char ch_state)
{
	if(ch_state)
		GPIOB->BSRRL = GPIO_BSRR_BS_14;
	else
		GPIOB->BSRRH = (GPIO_BSRR_BR_14>>16);
}
/*
 * Set the column address of RAM
 */
void lcd_set_col_addr(unsigned char ch_col)
{
	lcd_set_cd(COMMAND);
	spi_send_char(SET_COL_LSB | (ch_col & 0b1111));
	spi_send_char(SET_COL_MSB | (ch_col>>4));
}
/*
 * Set the page address of RAM
 * note: ch_page must be smaller than 16!
 */
void lcd_set_page_addr(unsigned char ch_page)
{
	lcd_set_cd(COMMAND);
	spi_send_char(SET_PAGE_LSB | (ch_page & 0b1111));
	spi_send_char(SET_PAGE_MSB | (ch_page>>4));
}

/*
 * Set the pattern number which is written into RAM
 */
void lcd_set_write_pattern(unsigned char ch_pat)
{
	lcd_set_cd(COMMAND);
	spi_send_char(SET_PAGE_MSB | ch_pat);
}
/*
 * Set which pattern is displayed.
 * Range of ch_pat: [0 3]!
 */
void lcd_set_pattern(unsigned char ch_pat)
{
	lcd_set_cd(COMMAND);
	spi_send_char(SET_DISP_PAT | DISP_PAT_DC5 | (ch_pat<<1));
}
/*
 * Switch display on and off
 */
void lcd_set_enable(unsigned char ch_state)
{
	lcd_set_cd(COMMAND);
	spi_send_char(SET_DISP_EN | ch_state);
}
/*
 * Set cursor in buffer
 */
void lcd_set_cursor(unsigned char ch_x, unsigned char ch_y)
{
	plcd_DOGXL->cursor_x = ch_x;
	plcd_DOGXL->cursor_y = ch_y;
}

/*
 * Send the lcd buffer to display
 * This function assumens that the RAM cursor is at the beginning of display RAM
 */
void lcd_send_buffer(void)
{
	lcd_set_cd(DATA);
	for(unsigned long l_count=0;l_count<(LCD_PIXEL_X*LCD_PIXEL_Y/8);l_count++)
	{
		spi_send_char(plcd_DOGXL->buffer[l_count]);
	}
}
/*
 * Write specific pixel to buffer. Also clears pixel
 */
void lcd_pixel2buffer(unsigned char ch_x, unsigned char ch_y, unsigned char ch_val)
{
	unsigned char ch_shift = ch_y % 8;
	if(ch_val)
		plcd_DOGXL->buffer[ch_x*(LCD_PIXEL_Y/8)+(ch_y/8)] |= (1<<ch_shift);
	else
		plcd_DOGXL->buffer[ch_x*(ch_y/8)] &= ~(1<<ch_shift);
}
/*
 * Write pixel data to buffer at cursor position.
 * Cursors aren't incremented after each access.
 */
void lcd_char2buffer(unsigned char ch_data)
{
	for (unsigned char ch_fonty = 0; ch_fonty<FONT_Y;ch_fonty++)
	{
		for (unsigned char ch_fontx = 0; ch_fontx<FONT_X;ch_fontx++)
		{
			lcd_pixel2buffer(,,font[ch_data][ch_fonty]);
		}
	}
}
/*
 * Clear buffer
 */
void lcd_clear_buffer(void)
{
	for(unsigned long l_count=0;l_count<(LCD_PIXEL_X*LCD_PIXEL_Y/8);l_count++)
	{
		plcd_DOGXL->buffer[l_count]=0;
	}
}
