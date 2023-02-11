/*
 * stm32uikit.h
 *
 *  Created on: Dec 17, 2021
 *      Author: k-omura
 */


#ifndef DEF_STM32UIKIT_H
#define DEF_STM32UIKIT_H

#include "stdio.h"
#include "string.h"
#include "bitmap.h"
#include "touch_2046.h"

#define SET_STRING 0
#define SET_PROGRESS 1
#define SET_BUTTON 2
#define SET_PIN 3
#define SET_SWITCH 4
#define SET_STATUS 5
#define SET_ANALOGMETER 6
#define SET_CIRCLRMETER 7
#define SET_GRAPH 8

void stm32uikit_componentSettings(uint8_t, uint8_t, uint8_t, uint16_t, uint8_t);

//output
void stm32uikit_roundProgress(uint16_t, uint16_t, uint16_t, uint16_t);
void stm32uikit_rectProgress(uint16_t, uint16_t, uint16_t, uint16_t);
void stm32uikit_analogMeter(uint16_t, uint16_t, uint16_t);
void stm32uikit_circleMeter(uint16_t, uint16_t, uint16_t, uint16_t);
void stm32uikit_status(uint16_t, uint16_t, uint8_t);
void stm32uikit_graph(uint16_t, uint16_t, int16_t, int16_t, int16_t *_data180);

//input-output
void stm32uikit_sllideBar(Coordinate_t, uint16_t, uint16_t, uint16_t, uint16_t *_val1000);
void stm32uikit_roundButton(Coordinate_t, uint16_t, uint16_t, uint16_t, uint8_t *_val);
void stm32uikit_switch(Coordinate_t, uint16_t, uint16_t, uint8_t *_val);

#endif
