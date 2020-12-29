/*
 * bitmap.c
 *
 *  Created on: May 27, 2020
 *      Author: omurakosuke
 */

#include "bitmap.h"
//#include "math.h"

void addPixel(int _x, int _y, int _width, uint8_t *_bitmap) {
	if(_y < 0) return;
	_bitmap[(_x / 8) + (((_width + 7) / 8) * _y)] |= (1 << (7 - _x % 8));
}

/* Bresenham's line algorithm */
void addLine(int _x0, int _y0, int _x1, int _y1, int _width, uint8_t *_bitmap) {
	int dx = (_x1 > _x0) ? (_x1 - _x0) : (_x0 - _x1);
	int dy = (_y1 > _y0) ? (_y1 - _y0) : (_y0 - _y1);
	int sx, sy, err, e2;

	if (_x0 < _x1) {
		sx = 1;
	} else {
		sx = -1;
	}
	if (_y0 < _y1) {
		sy = 1;
	} else {
		sy = -1;
	}
	err = dx - dy;

	while (1) {
		addPixel(_x0, _y0, _width, _bitmap);
		if ((_x0 == _x1) && (_y0 == _y1)) {
			break;
		}
		e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			_x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			_y0 += sy;
		}
	}
}
