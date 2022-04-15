/*
 * bitmap.h
 *
 *  Created on: Dec. 8, 2021
 *      Author: k-omura
 */

#ifndef SRC_BITMAP_H_
#define SRC_BITMAP_H_

#include "stdio.h"
struct bitmap_param_t{
	uint16_t width;
	uint16_t height;
	uint8_t background;
	uint8_t *bitmap;
};
extern struct bitmap_param_t bitmap_param;

typedef union {
	uint8_t raw;
	struct {
		unsigned int r :3;
		unsigned int g :3;
		unsigned int b :2;
	} rgb;
} ColorUnion8;

void bitmap_setparam(uint16_t, uint16_t, uint8_t, uint8_t *_bitmap);

void bitmap_pixel(uint16_t, uint16_t, uint8_t);
void bitmap_clear();
void bitmap_line(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);
void bitmap_bezier(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);

void bitmap_rect(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);
void bitmap_fillrect(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);

void bitmap_circle(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);
void bitmap_fillcircle(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);

void bitmap_roundrect(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);
void bitmap_fillroundrect(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);

void bitmap_stringBitmap(uint16_t, uint16_t, const char _character[], uint8_t, uint8_t, uint8_t);
void bitmap_characterBitmap8(uint16_t, uint16_t, char, uint8_t, uint8_t, uint8_t);
void bitmap_characterBitmap5(uint16_t, uint16_t, char, uint8_t);
void bitmap_terminal(const char _character[], uint8_t, uint8_t, uint8_t);

//---------------------------------
extern const unsigned char FONT8x8[][97][8];
extern const unsigned char FONT5x3[][2];


#endif /* SRC_BITMAP_H_ */
