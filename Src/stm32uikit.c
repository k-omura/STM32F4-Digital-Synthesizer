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
componentParam_t comp_cirMeter = {0x5c, 0x5c, 0x58, 50};
componentParam_t comp_graph = {0xda, 0xda, 0xff, 120};

void stm32uikit_componentSettings(uint8_t _comp_in, uint8_t _comp_out, uint8_t _comp_cont, uint16_t _height, uint8_t _component){
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
		case SET_CIRCLRMETER:
			comp_cirMeter = temp;
			break;
		case SET_GRAPH:
			comp_graph = temp;
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

	if(_val1000 > 1000){
		_val1000 = 1000;
	}

	float rad = M_PI * _val1000 / 1000;
	float cosdeg = cos(rad);
	float sindeg = sin(rad);

	uint16_t bar_x1 = center_x + 5 * cosdeg;
	uint16_t bar_x2 = center_x + -comp_anaMeter.height * cosdeg;
	uint16_t bar_y1 = center_y + 5 * sindeg;
	uint16_t bar_y2 = center_y + -comp_anaMeter.height * sindeg;

	bitmap_circle(center_x, center_y, comp_anaMeter.height, 1, 1, comp_anaMeter.out);
	bitmap_circle(center_x, center_y, comp_anaMeter.height, 2, 1, comp_anaMeter.out);
	bitmap_circle(center_x, center_y, comp_anaMeter.height - width_x, 1, 1, comp_anaMeter.in);
	bitmap_circle(center_x, center_y, comp_anaMeter.height - width_x, 2, 1, comp_anaMeter.in);
	bitmap_line(_x0, center_y, _x0 + width_x, center_y, comp_anaMeter.out);
	bitmap_line(center_x + comp_anaMeter.height, center_y, center_x + comp_anaMeter.height - width_x, center_y, comp_anaMeter.out);

	bitmap_line(bar_x1, bar_y1, bar_x2, bar_y2, comp_anaMeter.cont);
	bitmap_fillcircle(center_x, center_y, 2, 0, comp_anaMeter.out);
}

void stm32uikit_circleMeter(uint16_t _x0, uint16_t _y0, uint16_t _val1000, uint16_t _thickness){
	uint16_t width_x = comp_cirMeter.height / 2;
	uint16_t center_x = _x0 + width_x;
	uint16_t center_y = _y0 + width_x;

	if(_val1000 > 1000){
		_val1000 = 1000;
	}

	uint16_t val360 = 360 * _val1000 / 1000;
	uint16_t startAngle = 450 - val360;
	uint16_t stopAngle = 450;
	if((startAngle >= 360) && (stopAngle > 360)){
		startAngle -= 360;
		stopAngle -= 360;
	}

	uint16_t start360 = startAngle % 360;
	uint16_t stop360 = stopAngle % 360;

	int32_t f = 1 - width_x;
	uint16_t ddF_x = 1;
	int32_t ddF_y = -2 * width_x;
	uint16_t x = 0;
	int32_t y = width_x;

	uint16_t circumference[9] = {0};
	//circumference count
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		circumference[1]++;
	}

	for(uint8_t i = 2; i < 9; i++){
		circumference[i] = circumference[i - 1] + circumference[1];
		if((i % 2) == 0){
			circumference[i]++;
		}
	}
	circumference[1]++;

	uint16_t startCircumference = (uint16_t)((circumference[8] * startAngle / 360));
	uint16_t stopCircumference = (uint16_t)((circumference[8] * stopAngle / 360));
	uint16_t circumferenceBorder = stopCircumference;
	if((startAngle < 360) && (stopAngle > 360)){
		circumferenceBorder -= circumference[8];
	}

	//re-init
	f = 1 - width_x;
	ddF_x = 1;
	ddF_y = -2 * width_x;
	x = 0;
	y = width_x;

	if((start360 == 0) || (stop360 == 0) || ((startAngle <= 360) && (stopAngle >= 360))){
		bitmap_fillcircle(center_x + width_x, center_y, _thickness, 0 , comp_cirMeter.cont);
	}
	if((start360 == 90) || (stop360 == 90) || ((startAngle <= 90) && (stopAngle >= 90))){
		bitmap_fillcircle(center_x, center_y - width_x, _thickness, 0 , comp_cirMeter.cont);
	}
	if((start360 == 180) || (stop360 == 180) || ((startAngle <= 180) && (stopAngle >= 180))){
		bitmap_fillcircle(center_x - width_x, center_y, _thickness, 0 , comp_cirMeter.cont);
	}
	if((start360 == 270) || (stop360 == 270) || ((startAngle <= 270) && (stopAngle >= 270))){
		bitmap_fillcircle(center_x, center_y + width_x, _thickness, 0 , comp_cirMeter.cont);
	}

	circumference[3] = circumference[3] + circumference[1];
	circumference[5] = circumference[5] + circumference[1];
	circumference[7] = circumference[7] + circumference[1];
	circumference[1] = circumference[1] + circumference[1];
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		circumference[0]++;
		circumference[1]--;
		circumference[2]++;
		circumference[3]--;
		circumference[4]++;
		circumference[5]--;
		circumference[6]++;
		circumference[7]--;

		if(((startCircumference <= circumference[0]) && (stopCircumference >= circumference[0])) || ((stopAngle > 360) && (0 <= circumference[0]) && (circumferenceBorder >= circumference[0]))){
			bitmap_fillcircle(center_x + y, center_y - x, _thickness, 0 , comp_cirMeter.cont);
		}
		if(((startCircumference <= circumference[1]) && (stopCircumference >= circumference[1])) || ((stopAngle > 360) && (0 <= circumference[1]) && (circumferenceBorder >= circumference[1]))){
			bitmap_fillcircle(center_x + x, center_y - y, _thickness, 0 , comp_cirMeter.cont);
		}
		if(((startCircumference <= circumference[2]) && (stopCircumference >= circumference[2])) || ((stopAngle > 360) && (0 <= circumference[2]) && (circumferenceBorder >= circumference[2]))){
			bitmap_fillcircle(center_x - x, center_y - y, _thickness, 0 , comp_cirMeter.cont);
		}
		if(((startCircumference <= circumference[3]) && (stopCircumference >= circumference[3])) || ((stopAngle > 360) && (0 <= circumference[3]) && (circumferenceBorder >= circumference[3]))){
			bitmap_fillcircle(center_x - y, center_y - x, _thickness, 0 , comp_cirMeter.cont);
		}
		if(((startCircumference <= circumference[4]) && (stopCircumference >= circumference[4])) || ((stopAngle > 360) && (0 <= circumference[4]) && (circumferenceBorder >= circumference[4]))){
			bitmap_fillcircle(center_x - y, center_y + x, _thickness, 0 , comp_cirMeter.cont);
		}
		if(((startCircumference <= circumference[5]) && (stopCircumference >= circumference[5])) || ((stopAngle > 360) && (0 <= circumference[5]) && (circumferenceBorder >= circumference[5]))){
			bitmap_fillcircle(center_x - x, center_y + y, _thickness, 0 , comp_cirMeter.cont);
		}
		if(((startCircumference <= circumference[6]) && (stopCircumference >= circumference[6])) || ((stopAngle > 360) && (0 <= circumference[6]) && (circumferenceBorder >= circumference[6]))){
			bitmap_fillcircle(center_x + x, center_y + y, _thickness, 0 , comp_cirMeter.cont);
		}
		if(((startCircumference <= circumference[7]) && (stopCircumference >= circumference[7])) || ((stopAngle > 360) && (0 <= circumference[7]) && (circumferenceBorder >= circumference[7]))){
			bitmap_fillcircle(center_x + y, center_y + x, _thickness, 0 , comp_cirMeter.cont);
		}
	}
}

void stm32uikit_graph(uint16_t _x0, uint16_t _y0, int16_t _y_min, int16_t _y_max, int16_t *_data180){
	if(_y_min > _y_max){
		return;
	}

	char string[10];

	uint16_t bar_x1 = _x0 + 16;
	uint16_t bar_x2 = bar_x1 + 180;
	uint16_t bar_y1 = _y0;
	uint16_t bar_y2 = bar_y1 + comp_graph.height;

	sprintf(string, "%4d", _y_max);
	bitmap_stringBitmap(_x0, bar_y1, string, 0, 0, comp_graph.out);
	sprintf(string, "%4d", _y_min);
	bitmap_stringBitmap(_x0, bar_y2 - 5, string, 0, 0, comp_graph.out);

	uint16_t widthOfGraphDisplay = _y_max - _y_min;
	uint16_t data;

	for(uint16_t i = 0; i < 180; i++){
		if((_y_min >= _data180[i]) || (_y_max < _data180[i])){
			continue;
		}
		data = (uint16_t)(comp_graph.height * (_data180[i] - _y_min) / widthOfGraphDisplay);
		data = comp_graph.height - data;
		bitmap_pixel(bar_x1 + i, bar_y1 + data, comp_graph.cont);
	}

	bitmap_line(bar_x1, bar_y1, bar_x1, bar_y2, comp_graph.in);
	bitmap_line(bar_x1, bar_y2, bar_x2, bar_y2, comp_graph.in);
	return;
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
