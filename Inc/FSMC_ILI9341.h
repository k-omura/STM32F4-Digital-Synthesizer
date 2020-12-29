/*
 * FSMC_ILI9341.h
 *
 *  Created on: May 27, 2020
 *      Author: k-omura
 *
 * FSMC 16bit
 */


//List of includes
#include "main.h"
#include <stdio.h>

//LCD dimensions defines
#define ILI9341_WIDTH 240
#define ILI9341_HEIGHT 320
#define ILI9341_PIXEL_COUNT	(ILI9341_WIDTH * ILI9341_HEIGHT)

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

//List of colors
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_ORANGE      0xFD20      /* 255, 165,   0 */
#define TFT_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define TFT_PINK        0xF81F
#define TFT_WHITE       0b1111011111011110
#define TFT_yellow      0b1110011110000000
#define TFT_blue        0b0011000110011111
#define TFT_green       0b0011011110000011
#define Major_white     0b0111101111101111
#define miner_white     0b0011100111100111

typedef union {
	uint16_t raw;
	struct {
		unsigned int r :5;
		unsigned int g :6;
		unsigned int b :5;
	} rgb;
	struct {
		unsigned int high :8;
		unsigned int low :8;
	} split;
} ColorUnion;

//functions
void ILI9341_sendCommand16(uint16_t);
void ILI9341_sendData16(uint16_t);
void ILI9341_sendCommand8(uint8_t);
void ILI9341_sendData8(uint8_t);
void ILI9341_init(void);
void ILI9341_setRotation(uint16_t);
void ILI9341_setRect(uint16_t, uint16_t, uint16_t, uint16_t);

void ILI9341_drawPixel(uint16_t, uint16_t, uint16_t); //Draw single pixel to ILI9341
void ILI9341_fill_all(uint16_t); //Fill entire ILI9341 with color
void ILI9341_fill_Rect(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t color);
void ILI9341_printImage(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t*);

void ILI9341_addString(uint16_t, uint16_t, const char _character[], uint16_t, uint8_t);
void ILI9341_string(uint16_t, uint16_t, const char _character[], uint16_t, uint16_t, uint8_t);
void add_character8(uint16_t, uint16_t, char, uint16_t);
void add_character5(uint16_t, uint16_t, char, uint16_t);
void character8(uint16_t, uint16_t, char, uint16_t, uint16_t);
void character5(uint16_t, uint16_t, char, uint16_t, uint16_t);

//---------------------------------
extern const unsigned char FONT8x8[][8];
extern const unsigned char FONT5x3[][2];
