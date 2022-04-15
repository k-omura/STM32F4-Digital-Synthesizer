/*
 * stm32uikit.c
 *
 *  Created on: Dec 17, 2021
 *      Author: k-omura
 */

#include "stm32uikit.h"
#include "math.h"

#define TOUCH_Z_MIN 300

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

typedef struct {
	uint8_t in;
	uint8_t out;
	uint8_t cont;
	uint16_t height;
} componentParam_t;

componentParam_t comp_string = {0xff, 0xff, 0xff, 30};
componentParam_t comp_pin = {0xda, 0xff, 0x96, 7};
componentParam_t comp_progress = {0x04, 0x58, 0x3d, 6};
componentParam_t comp_button = {0x14, 0x3d, 0x58, 30};
componentParam_t comp_switch = {0x58, 0xda, 0xff, 22};
componentParam_t comp_status = {0x5c, 0xda, 0xe1, 7};
componentParam_t comp_anaMeter = {0x58, 0x31, 0xe1, 40};

void stm32uikit_componentSetgtings(uint8_t _comp_in, uint8_t _comp_out, uint8_t _comp_cont, uint16_t _height, uint8_t _component){
	componentParam_t temp = {_comp_in, _comp_out, _comp_cont, _height};
	switch(_component){
		case SET_STRING:
			comp_string = temp;
			break;
		case SET_PROGRESS:
			comp_progress = temp;
			break;
		case SET_BUTTON:
			comp_button = temp;
			break;
		case SET_PIN:
			comp_pin = temp;
			break;
		case SET_SWITCH:
			comp_switch = temp;
			break;
		case SET_STATUS:
			comp_status = temp;
			break;
		case SET_ANALOGMETER:
			comp_anaMeter = temp;
			break;
	}
}

void stm32uikit_roundProgress(uint16_t _x0, uint16_t _y0, uint16_t _width, uint16_t _val1000){
	uint16_t x1 = _x0 + _width;
	uint16_t y1 = _y0 + comp_progress.height;
	if((_width < comp_progress.height) || (_width < 0) || (comp_progress.height < 0)){
		return;
	}
	if(_val1000 > 1000){
		_val1000 = 1000;
	}

	uint16_t round = comp_progress.height / 2;
	bitmap_roundrect(_x0, _y0, x1, y1, round, 1, comp_progress.out);
	bitmap_fillroundrect(_x0 + 1, _y0 + 1, x1 - 1, y1 - 1, round - 1, comp_progress.in);
	bitmap_fillroundrect(_x0 + 1, _y0 + 1, (_x0 + comp_progress.height - 1 + ((_width - comp_progress.height) * _val1000 / 1000)), y1 - 1, round - 1, comp_progress.cont);
}


void stm32uikit_rectProgress(uint16_t _x0, uint16_t _y0, uint16_t _width, uint16_t _val1000){
	uint16_t x1 = _x0 + _width;
	uint16_t y1 = _y0 + comp_progress.height;
	if((_width < comp_progress.height) || (_width < 0) || (comp_progress.height < 0)){
		return;
	}
	if(_val1000 > 1000){
		_val1000 = 1000;
	}

	bitmap_rect(_x0, _y0, x1, y1, 1, comp_progress.out);
	bitmap_fillrect(_x0 + 1, _y0 + 1, x1 - 1, y1 - 1, comp_progress.in);
	bitmap_fillrect(_x0 + 1, _y0 + 1, (_x0 + 1 + ((_width - 2) * _val1000 / 1000)), y1 - 1, comp_progress.cont);
}

void stm32uikit_analogMeter(uint16_t _x0, uint16_t _y0, uint16_t _val1000){
	uint16_t center_x = _x0 + comp_anaMeter.height;
	uint16_t center_y = _y0 + comp_anaMeter.height;
	uint16_t width_x = comp_anaMeter.height / 4;

	float rad = M_PI * _val1000 / 1000;
	float cosdeg = cos(rad);
	float sindeg = sin(rad);

	uint16_t bar_x1 = center_x + 5 * cosdeg;
	uint16_t bar_x2 = center_x + -comp_anaMeter.height * cosdeg;
	uint16_t bar_y1 = center_y + 5 * sindeg;
	uint16_t bar_y2 = center_y + -comp_anaMeter.height * sindeg;

	if(_val1000 > 1000){
		_val1000 = 1000;
	}

	bitmap_circle(center_x, center_y, comp_anaMeter.height, 1, 1, comp_anaMeter.out);
	bitmap_circle(center_x, center_y, comp_anaMeter.height, 2, 1, comp_anaMeter.out);
	bitmap_circle(center_x, center_y, comp_anaMeter.height - width_x, 1, 1, comp_anaMeter.in);
	bitmap_circle(center_x, center_y, comp_anaMeter.height - width_x, 2, 1, comp_anaMeter.in);
	bitmap_line(_x0, center_y, _x0 + width_x, center_y, comp_anaMeter.out);
	bitmap_line(center_x + comp_anaMeter.height, center_y, center_x + comp_anaMeter.height - width_x, center_y, comp_anaMeter.out);

	bitmap_line(bar_x1, bar_y1, bar_x2, bar_y2, comp_anaMeter.cont);
	bitmap_fillcircle(center_x, center_y, 2, 0, comp_anaMeter.out);
}

void stm32uikit_status(uint16_t _x0, uint16_t _y0, uint8_t _status){
	bitmap_fillcircle(_x0, _y0, comp_status.height, 0, comp_status.out);

	uint8_t col;
	switch(_status){
		case 1:
			col = comp_status.in;
			break;
		case 2:
			col = comp_status.cont;
			break;
		default:
			col = comp_status.out;
			break;
	}
	bitmap_fillcircle(_x0, _y0, comp_status.height - 1, 0, col);
}

void stm32uikit_sllideBar(Coordinate_t _touch, uint16_t _x0, uint16_t _y0, uint16_t _width, uint16_t *_val1000){
	uint16_t progress_radius = (comp_progress.height / 2);
	uint16_t center_y = _y0 + progress_radius;

	stm32uikit_roundProgress(_x0, _y0, _width, (*_val1000));
	bitmap_fillcircle((_x0 + progress_radius + ((_width - comp_progress.height) * (*_val1000) / 1000)), center_y, comp_pin.height, 0, comp_pin.in);
	bitmap_circle((_x0 + progress_radius + ((_width - comp_progress.height) * (*_val1000) / 1000)), center_y, comp_pin.height, 0, 1, comp_pin.out);

	if(TOUCH_Z_MIN > _touch.z){
		return;
	}
	if((_touch.x < _x0) || (_touch.x > (_x0 + _width))){
		return;
	}
	if((_touch.y < (center_y - comp_pin.height)) || (_touch.y > (center_y + comp_pin.height))){
		return;
	}

	*_val1000 = 1000 * (_touch.x - _x0) / (_width);
	return;
}

void stm32uikit_roundButton(Coordinate_t _touch, uint16_t _x0, uint16_t _y0, uint16_t _width, uint8_t *_val){
	uint8_t comp_inside = ((*_val) == 1) ? (comp_button.cont) : (comp_button.in);
	bitmap_roundrect(_x0, _y0, _x0 + _width, _y0 + comp_button.height, 8, 1, comp_button.out);
	bitmap_fillroundrect(_x0 + 1, _y0 + 1, _x0 + _width - 1, _y0 + comp_button.height - 1, 7, comp_inside);


	if((TOUCH_Z_MIN > _touch.z) || (_touch.x < _x0) || (_touch.x > (_x0 + _width)) || (_touch.y < _y0) || (_touch.y > (_y0 + comp_button.height))){
		*_val = ((*_val) == 1) ? (2) : (0);
		return;
	}

	*_val = 1;
}

void stm32uikit_switch(Coordinate_t _touch, uint16_t _x0, uint16_t _y0, uint8_t *_val){
	uint8_t comp_inside;
	uint16_t pin_x;
	uint16_t round = comp_switch.height / 2;
	uint16_t width = round * 3.5;

	if(((*_val) % 2) == 1){
		comp_inside = comp_switch.in;
		pin_x = _x0 + width - round;
	}else{
		comp_inside = comp_switch.out;
		pin_x = _x0 + round;
	}

	bitmap_fillroundrect(_x0, _y0, _x0 + width, _y0 + comp_switch.height, round, comp_inside);
	bitmap_fillcircle(pin_x, _y0 + round, round - 2, 0, comp_switch.cont);

	if((TOUCH_Z_MIN > _touch.z) || (_touch.x < _x0) || (_touch.x > (_x0 + width)) || (_touch.y < _y0) || (_touch.y > (_y0 + comp_switch.height))){
		switch(*_val){
			case 2:
				*_val = 1;
				break;
			case 3:
				*_val = 0;
				break;
			default:
				break;
		}
		return;
	}

	switch(*_val){
		case 0:
			*_val = 2;
			break;
		case 1:
			*_val = 3;
			break;
		default:
			break;
	}
}
