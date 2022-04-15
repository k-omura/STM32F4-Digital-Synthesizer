/*
 * FSMC_ILI9341.h
 *
 *  Created on: Dec 8, 2021
 *      Author: k-omura
 *
 * FSMC 8bit
 */


//List of includes
#include "main.h"
#include <stdio.h>

//LCD dimensions defines
#define ILI9341_WIDTH 240
#define ILI9341_HEIGHT 320
#define ILI9341_PIXEL_COUNT	(ILI9341_WIDTH * ILI9341_HEIGHT)

#define ILI9341_SPLIT_NUM 80

//ILI9341 commands
#define LCD_REGSELECT_BIT 18 //LCD Register Select. if A18 -> 18.
#define LCD_ADDR_BASE 0x60000000
#define LCD_ADDR_DATA (LCD_ADDR_BASE + (1 << (LCD_REGSELECT_BIT + 2)) - 2)

#define ILI9341_RESET 0x01
#define ILI9341_SLEEP_OUT 0x11
#define ILI9341_GAMMA 0x26
#define ILI9341_DISPLAY_OFF 0x28
#define ILI9341_DISPLAY_ON 0x29
#define ILI9341_COLUMN_ADDR 0x2A
#define ILI9341_PAGE_ADDR 0x2B
#define ILI9341_GRAM 0x2C
#define ILI9341_TEARING_OFF 0x34
#define ILI9341_TEARING_ON 0x35
#define ILI9341_DISPLAY_INVERSION 0xb4
#define ILI9341_MAC 0x36
#define ILI9341_PIXEL_FORMAT 0x3A
#define ILI9341_WDB 0x51
#define ILI9341_WCD 0x53
#define ILI9341_RGB_INTERFACE 0xB0
#define ILI9341_FRC 0xB1
#define ILI9341_BPC 0xB5
#define ILI9341_DFC 0xB6
#define ILI9341_Entry_Mode_Set 0xB7
#define ILI9341_POWER1 0xC0
#define ILI9341_POWER2 0xC1
#define ILI9341_VCOM1 0xC5
#define ILI9341_VCOM2 0xC7
#define ILI9341_POWERA 0xCB
#define ILI9341_POWERB 0xCF
#define ILI9341_PGAMMA 0xE0
#define ILI9341_NGAMMA 0xE1
#define ILI9341_DTCA 0xE8
#define ILI9341_DTCB 0xEA
#define ILI9341_POWER_SEQ 0xED
#define ILI9341_3GAMMA_EN 0xF2
#define ILI9341_INTERFACE 0xF6
#define ILI9341_PRC 0xF7
#define ILI9341_VERTICAL_SCROLL 0x33

#define ILI9341_MEMCONTROL 0x36
#define ILI9341_MADCTL_MY 0x80
#define ILI9341_MADCTL_MX 0x40
#define ILI9341_MADCTL_MV 0x20
#define ILI9341_MADCTL_ML 0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH 0x04

//functions
void ILI9341_sendCommand16(uint16_t);
void ILI9341_sendData16(uint16_t);
void ILI9341_sendCommand8(uint8_t);
void ILI9341_sendData8(uint8_t);

void ILI9341_init(void);
void ILI9341_setRotation(uint8_t);

void ILI9341_setRect(uint16_t, uint16_t, uint16_t, uint16_t);
void ILI9341_printBitmap(uint8_t*);
void ILI9341_printBitmap_split(uint8_t*, uint8_t);

extern const uint16_t col8to16[256];
