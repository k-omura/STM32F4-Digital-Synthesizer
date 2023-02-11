/*
 * elementary_wave.h
 *
 *  Created on: May 28, 2020
 *      Author: k-omura
 */

#include "main.h"
#include "stdio.h"
#include "math.h"

#ifndef ARM_MATH_CM4
#define ARM_MATH_CM4
#include "arm_math.h"
#endif

#include <synthesizer_parameters.h>

#define M_PIF32 3.141592653589793f

//*
#define SYNTH_NUM_SAMPLING 1024 //
#define SYNTH_NUM_I2SBUFF 2048 //buff size for i2s (L+R)
#define SYNTH_HALF_NUM_SAMPLING 512 //
#define SYNTH_PRE_SAMPLE 100 //-
#define SYNTH_GND_LEVEL 0x7ff //12bit dac
#define SYNTH_AMP_MAX 0xfff //12bit dac
#define SYNTH_MAX_FREQ 20000 //Hz
#define SYNTH_MAX_FREQ_NOTE 34579 //Hz
#define SYNTH_MIN_FREQ 20 //Hz
#define SYNTH_SAMPLE_FREQ 40000 //Hz
#define SYNTH_SAMPLE_ALL SYNTH_NUM_SAMPLING + SYNTH_PRE_SAMPLE //-
#define SYNTH_MAX_CHORD 13 //
#define SYNTH_DOUBLE_PI 6.283185307179586f

#define SYNTH_STATUS_WAIT 0
#define SYNTH_STATUS_HALF 1
#define SYNTH_STATUS_DONE 2
#define SYNTH_STATUS_LFOC 3
#define SYNTH_STATUS_CHRD 4
#define SYNTH_STATUS_POST 5

#define SYNTH_IND_WOW 0
#define SYNTH_IND_VIB 1
#define SYNTH_IND_TRE 2
#define SYNTH_IND_FIL 3
#define SYNTH_IND_SIG 4

#define SYNTH_SIG_SIN  0
#define SYNTH_SIG_SQU  1
#define SYNTH_SIG_SAW1 2
#define SYNTH_SIG_TRI  3
#define SYNTH_SIG_SAW2 4

void set_synth_para(uint16_t*, uint16_t*);
//void set_synth_para(uint16_t*, uint16_t*, RNG_HandleTypeDef*);
uint8_t synth_process();
float32_t lfo_waveform(float32_t*, uint32_t, uint32_t, uint32_t);

void add_signal(float32_t*, float32_t*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
void add_sin(float32_t*, float32_t*, uint32_t, uint32_t);
void add_square(float32_t*, float32_t*, uint32_t, uint32_t);
void add_triangle(float32_t*, float32_t*, uint32_t, uint32_t);
void add_sawtooth(float32_t*, float32_t*, uint32_t, uint32_t);
void add_noise(float32_t*, uint32_t, uint32_t);

void float2uint(float32_t*, uint16_t*, uint32_t, uint32_t);
void calc_lpf_coeffs(float32_t*, float32_t, float32_t);
void calc_hpf_coeffs(float32_t*, float32_t, float32_t);
void calc_amp_char(uint16_t*, float32_t, float32_t);

extern const float32_t log_axis[240];
