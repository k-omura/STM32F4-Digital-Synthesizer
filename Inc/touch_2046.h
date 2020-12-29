/*
 * touch_2046.h
 *
 *  Created on: Jun 5, 2020
 *      Author: omurakosuke
 */

#include "main.h"
#include <stdio.h>

#define XPT2046_ROTATION 1
#define XPT2046_HEIGHT 320
#define XPT2046_WIDTH 240

#define READ_BASE 0b10000011
#define READ_X    0b01010000
#define READ_Y    0b00010000
#define READ_Z1   0b00110000
#define READ_Z2   0b01000000

#define XPT2046_MAX 0x0fff
#define XPT2046_SAMPLE 30

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t z;
} Coordinate_t;

typedef struct {
	uint16_t minX;
	uint16_t minY;
	uint16_t maxX;
	uint16_t maxY;
} Touch_calib_t;

//void xpt2046_init(SPI_HandleTypeDef*);
Coordinate_t xpt2046_read(SPI_HandleTypeDef*, Touch_calib_t);
