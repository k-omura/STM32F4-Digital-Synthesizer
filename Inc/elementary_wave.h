/*
 * elementary_wave.h
 *
 *  Created on: May 28, 2020
 *      Author: omurakosuke
 */

#include "stdio.h"
#include "math.h"

#ifndef ARM_MATH_CM4
#define ARM_MATH_CM4
#include "arm_math.h"
#endif

//#include "math.h"
//#define M_PI 3.141592653589

#define NUM_SAMPLING 1024 //1024 * 25us = 25.6ms
#define HALF_NUM_SAMPLING 512 //
#define GND_LEVEL 2048 //MAX 4096
#define AMP_MAX 4096 //MAX 4096
#define MAX_FREQ 20000 //Hz
#define MAX_FREQ_NOTE 34579 //Hz
#define MIN_FREQ 20 //Hz
#define SAMPLE_FREQ 40000 //Hz
#define SAMPLE_CYCLE 25 //us
#define PRE_SAMPLE 100 //-
#define SAMPLE_ALL NUM_SAMPLING + PRE_SAMPLE //-

void add_sin(float *_audio, uint32_t, float, uint16_t, uint16_t);
void add_square(float *_audio, uint32_t, float, uint16_t, uint16_t);
void add_triangle(float *_audio, uint32_t, float, uint16_t, uint16_t);
void add_sawtooth(float *_audio, uint32_t, float, uint16_t, uint16_t);

void float2uint(float *_in, uint16_t *_out, uint16_t, uint16_t);
void calc_lpf_coeffs(float32_t *_coeffs, float, float);
void calc_amp_char(uint16_t *_amp, float, float);

extern const float log_axis[240];
