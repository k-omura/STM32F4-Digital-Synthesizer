/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define ARM_MATH_CM4
#include "arm_math.h"

#include <elementary_wave.h>
#include <synthesizer_parameters.h>

#include <FSMC_ILI9341.h>
#include <bitmap.h>
#include <touch_2046.h>
#include <stm32uikit.h>

#include <otakuro_logo.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef union {
	uint16_t raw;
	struct {
		unsigned int low :8;
		unsigned int high :8;
	} split;
} transmit16;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define XPT2046_MIN_RAW_X 230
#define XPT2046_MAX_RAW_X 3900
#define XPT2046_MIN_RAW_Y 290
#define XPT2046_MAX_RAW_Y 3870

#define COLOR_BACKGROUND 0x00
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

RNG_HandleTypeDef hrng;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi3_rx;

TIM_HandleTypeDef htim6;

DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */
//uint8_t spi_controler_tx[4] = {0};
uint8_t frameBuffer[ILI9341_PIXEL_COUNT] = {0};

uint8_t spi_controler_rx[4];
uint16_t control_value[SIZE_PARAMATER] = { 0 };
uint8_t spiRecieving = 1;

uint8_t touch_it = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FSMC_Init(void);
static void MX_DAC_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM6_Init(void);
static void MX_SPI1_Init(void);
static void MX_RNG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if((spiRecieving == 0) && (GPIO_PIN_4 == GPIO_Pin)){
		HAL_SPI_Receive_DMA(&hspi1, spi_controler_rx, 4);
		spiRecieving = 1;
	}
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
	if(hspi == &hspi1){
		transmit16 data;
		data.split.low = spi_controler_rx[0];
		data.split.high = spi_controler_rx[1];
		uint16_t parameter = data.raw;

		if((parameter >= 0) && (parameter < SIZE_PARAMATER)){
			data.split.low = spi_controler_rx[2];
			data.split.high = spi_controler_rx[3];
			control_value[parameter] = data.raw;
		}

		spiRecieving = 0;
		MX_SPI1_Init();
	}else if(hspi == &hspi3){
		transmit16 data;
		data.split.low = spi_controler_rx[0];
		data.split.high = spi_controler_rx[1];
		uint16_t parameter = data.raw;

		if((parameter >= 0) && (parameter < SIZE_PARAMATER)){
			data.split.low = spi_controler_rx[2];
			data.split.high = spi_controler_rx[3];
			control_value[parameter] = data.raw;
		}

		HAL_SPI_Receive_DMA(&hspi3, spi_controler_rx, 4);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_DAC_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_TIM6_Init();
  MX_SPI1_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */

  // Touch init
/*
  Coordinate_t touch_s3uikit;
  Touch_calib_t touch_cal;
	touch_cal.minX = XPT2046_MIN_RAW_X;
	touch_cal.minY = XPT2046_MIN_RAW_Y;
	touch_cal.maxX = XPT2046_MAX_RAW_X;
	touch_cal.maxY = XPT2046_MAX_RAW_Y;
	*/

    // LCD init
  	ILI9341_init();
  	ILI9341_setRotation(1);

  	//----------bitmap init
	char string[30];
	bitmap_setparam(ILI9341_WIDTH, ILI9341_HEIGHT, COLOR_BACKGROUND, frameBuffer);
	bitmap_clear();

	//color test
	/*
	for (uint16_t i = 0; i < 256; i++) {
		bitmap_fillrect(15 * (i % 16), ((uint16_t) (i / 16) * 15), 15 * (i % 16) + 14, ((uint16_t) (i / 16) * 15) + 14, i);
	}
	ILI9341_printBitmap(frameBuffer);
	bitmap_clear();
	HAL_Delay(2000);
	*/

	//otakuro animation
	for (uint8_t i = 0; i < 13; i++) {
		bitmap_clear();
		bitmap_animation_4bit(&OTAKURO[i][0][0], OTAKURO_color_map, 72, 100, 48, 64);
		ILI9341_printBitmap(frameBuffer);
		HAL_Delay(100);
	}
	bitmap_stringBitmap(40, 180, "STM32F407 Synthesizer", 1, 1, 0xff);
	bitmap_stringBitmap(165, 310, "V20230205", 1, 1, 0xff);
	ILI9341_printBitmap(frameBuffer);
	HAL_Delay(2000);

	uint8_t color_behavior[] = {0x0b, 0x58, 0xf8, 0xe4, 0xff};
	uint8_t color_signal[] = {0xde, 0x9f, 0xef, 0xfd};

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	//input spi
	HAL_SPI_Receive_DMA(&hspi3, spi_controler_rx, 4);
	spiRecieving = 0; //start spi1 read
	uint8_t displaySplitIndex = 0; //Split screen display index

	//start timer and DAC
	uint16_t audio_dac[SYNTH_NUM_SAMPLING] = { 0 };
	HAL_TIM_GenerateEvent(&htim6, TIM_EVENTSOURCE_UPDATE);
	HAL_TIM_Base_Start(&htim6);
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) audio_dac, SYNTH_NUM_SAMPLING, DAC_ALIGN_12B_R);

	//parameter setting
	set_synth_para(audio_dac, control_value);
	uint8_t synth_process_status = 1;

	//init value(for test)
	/*
	control_value[S1_ENABLE] = 1;
	control_value[S1_FREQ] = 17664; //440Hz
	control_value[S1_SIN] = 100;
	control_value[S1_SQU] = 0;
	control_value[S1_TRI] = 0;
	control_value[S1_SAW] = 0;
	//*/
	//control_value[S1_SIN] = 400;

	//default value
	control_value[M_VOLUME] = 50;
	control_value[SYNC_WAVFORM] = 1;
	control_value[LPF_CUTOFF] = 34579; //20kHz
	control_value[LPF_Q] = 80;
	control_value[PITCH] = 256;
	control_value[VIB_AMP] = 0x0100;
	control_value[TRE_AMP] = 400;

	control_value[S1_ENABLE] = 0;
	control_value[S2_ENABLE] = 0;
	control_value[S3_ENABLE] = 0;
	control_value[S4_ENABLE] = 0;
	control_value[S5_ENABLE] = 0;
	control_value[S6_ENABLE] = 0;
	control_value[S7_ENABLE] = 0;
	control_value[S8_ENABLE] = 0;
	control_value[S9_ENABLE] = 0;
	control_value[S10_ENABLE] = 0;

	control_value[S1_FREQ] = 72 << 8;
	control_value[S2_FREQ] = 74 << 8;
	control_value[S3_FREQ] = 75 << 8;
	control_value[S4_FREQ] = 76 << 8;
	control_value[S5_FREQ] = 78 << 8;
	control_value[S6_FREQ] = 80 << 8;
	control_value[S7_FREQ] = 82 << 8;
	control_value[S8_FREQ] = 83 << 8;
	control_value[S9_FREQ] = 84 << 8;
	control_value[S10_FREQ] = 86 << 8;

	control_value[EG_ATTACK_TIM] = 100;
	control_value[EG_ATTACK_LEVEL] = 150;
	control_value[EG_DECAY_TIME] = 150;
	control_value[EG_RELEASE_TIME] = 500;

	while (1) {
		//input touch
		/*
		touch_s3uikit = xpt2046_read(&hspi2, touch_cal);
		if(touch_s3uikit.z < 100){
			touch_s3uikit.x = 0;
			touch_s3uikit.y = 0;
			touch_s3uikit.z = 0;
		}

		if ((touch_s3uikit.z > 300)) {
			if (touch_s3uikit.y < 150) {
				control_value[LPF_CUTOFF] = 150 * touch_s3uikit.x;
				control_value[LPF_Q] = 40 + (2 * touch_s3uikit.y / 3);
			} else if (touch_s3uikit.y < 180) {
			} else if (touch_s3uikit.y < 300) {
				control_value[S1_FREQ] = 144 * (239 - touch_s3uikit.x);
				uint16_t amp = ((touch_s3uikit.y - 180) * 2048) / 120;
				control_value[S1_SIN] = amp;
			} else if (touch_s3uikit.y < ILI9341_HEIGHT) {
			}

			touch_it = 0;
		}
		*/

		synth_process_status = synth_process();

		if(synth_process_status == 1) {
			if(displaySplitIndex < ILI9341_SPLIT_NUM){
				ILI9341_printBitmap_split(frameBuffer, displaySplitIndex++);
			}else if(displaySplitIndex == ILI9341_SPLIT_NUM){
				//set frame buffer
				bitmap_clear();

				uint16_t dis_y_adsr = 20;
				uint16_t dis_x_adsr = 30;
				uint16_t dis_x1, dis_x2, dis_y1, dis_y2;

				bitmap_stringBitmap(5, dis_y_adsr - 10, "ADSR", 1, 1, 0xff);
				bitmap_line(20, dis_y_adsr + 100, 220, dis_y_adsr + 100, 0xda); //0 line
				bitmap_stringBitmap(188, dis_y_adsr + 103, "[ms]", 1, 1, 0xda);

				dis_x1 = dis_x_adsr + (20 * ((control_value[EG_ATTACK_TIM] > 1000)?(1000):(control_value[EG_ATTACK_TIM])) / 1000);
				dis_y1 = dis_y_adsr + 100 - (uint16_t)(50 * control_value[EG_ATTACK_LEVEL] / 100);
				bitmap_line(dis_x_adsr, dis_y_adsr + 100, dis_x1, dis_y1, 0x1f); //A line
				sprintf(string, "%4d", control_value[EG_ATTACK_TIM]);
				bitmap_stringBitmap(dis_x_adsr, dis_y_adsr + 103, string, 0, 0, 0x1f);
				sprintf(string, "%d.%02d", (uint8_t) (control_value[EG_ATTACK_LEVEL] / 100), (uint8_t) (control_value[EG_ATTACK_LEVEL] * 100) % 100);
				bitmap_stringBitmap(dis_x1 - 20, dis_y1, string, 0, 0, 0x1f);

				dis_x2 = dis_x1 + (20 * ((control_value[EG_DECAY_TIME] > 1000)?(1000):(control_value[EG_DECAY_TIME])) / 1000);
				dis_y2 = dis_y_adsr + 50;
				bitmap_line(dis_x1, dis_y1, dis_x2, dis_y2, 0x9d); //D line
				sprintf(string, "%4d", control_value[EG_DECAY_TIME]);
				bitmap_stringBitmap(dis_x1, dis_y2 + 2, string, 0, 0, 0x9d);

				dis_x1 = dis_x2 + 20;
				bitmap_line(dis_x2, dis_y2, dis_x1, dis_y2, 0xf4); //S line

				dis_x2 = dis_x1 + (120 * ((control_value[EG_RELEASE_TIME] > 6000)?(6000):(control_value[EG_RELEASE_TIME])) / 6000);
				bitmap_line(dis_x1, dis_y2, dis_x2, dis_y_adsr + 100, 0xfd); //R line
				sprintf(string, "%5d", control_value[EG_RELEASE_TIME]);
				bitmap_stringBitmap(dis_x1, dis_y2 - 10, string, 0, 0, 0xfd);

				//Noise
				if(control_value[N1_ENABLE] > 0){
					bitmap_rect(100, 10, 102, 12, 1, 0xff);
				}
				if(control_value[N2_ENABLE] > 0){
					bitmap_rect(103, 10, 105, 12, 1, 0xff);
				}
				if(control_value[N3_ENABLE] > 0){
					bitmap_rect(106, 10, 107, 12, 1, 0xff);
				}

				displaySplitIndex++;
			}else{
				//display para all
				uint16_t dis_y_freq = 250;
				uint16_t dis_y_amp = 160;
				uint16_t dis_x_freq, dis_x_amp;
				uint16_t dis_width_lfo_vib = 0;
				uint16_t dis_width_lfo_tre = 0;
				uint16_t dis_width_lfo_wow = 0;

				//title
				bitmap_stringBitmap(45, dis_y_amp - 20, "LPF", 1, 1, color_behavior[SYNTH_IND_FIL]);
				bitmap_stringBitmap(85, dis_y_amp - 20, "WOW", 1, 1, color_behavior[SYNTH_IND_WOW]);
				bitmap_stringBitmap(125, dis_y_amp - 20, "VIB", 1, 1, color_behavior[SYNTH_IND_VIB]);
				bitmap_stringBitmap(165, dis_y_amp - 20, "TRE", 1, 1, color_behavior[SYNTH_IND_TRE]);

				//behavior formtype
				uint16_t behavior_formtype = WOW_FORM;
				uint8_t x_formtype = 85 + 8 * 3;
				for(uint8_t i = 0; i < 3; i++){
					switch(control_value[behavior_formtype]){
						case 0:
							bitmap_stringBitmap(x_formtype, dis_y_amp - 20, "~", 1, 1, color_behavior[i]);
							break;
						case 1:
							bitmap_line(x_formtype, dis_y_amp - 20, x_formtype + 4, dis_y_amp - 20, color_behavior[i]);
							bitmap_line(x_formtype + 4, dis_y_amp - 20, x_formtype + 4, dis_y_amp - 13, color_behavior[i]);
							bitmap_line(x_formtype + 4, dis_y_amp - 13, x_formtype + 8, dis_y_amp - 13, color_behavior[i]);
							break;
						case 2:
							bitmap_line(x_formtype, dis_y_amp - 20, x_formtype + 7, dis_y_amp - 13, color_behavior[i]);
							break;
						case 3:
							bitmap_line(x_formtype, dis_y_amp - 13, x_formtype + 7, dis_y_amp - 20, color_behavior[i]);
							break;
						default:
							break;
					}
					behavior_formtype += 3;
					x_formtype += 40;
				}

				//amp. axis
				bitmap_stringBitmap(5, dis_y_amp - 10, "AMP.", 1, 1, 0xff);
				bitmap_stringBitmap(45, dis_y_amp - 10, "SIN", 1, 1, color_signal[SYNTH_SIG_SIN]);
				bitmap_stringBitmap(85, dis_y_amp - 10, "SQU", 1, 1, color_signal[SYNTH_SIG_SQU]);
				bitmap_stringBitmap(125, dis_y_amp - 10, "SAW", 1, 1, color_signal[SYNTH_SIG_SAW1]);
				bitmap_stringBitmap(165, dis_y_amp - 10, "TRI", 1, 1, color_signal[SYNTH_SIG_TRI]);

				//freq. axis
				bitmap_stringBitmap(5, dis_y_freq - 10, "FREQ.", 1, 1, 0xff);
				sprintf(string, "[Q:%2d.%02d]", (uint8_t)(control_value[LPF_Q] / 100), (uint8_t) (control_value[LPF_Q]) % 100);
				bitmap_stringBitmap(140, dis_y_freq - 10, string, 1, 1, color_behavior[SYNTH_IND_FIL]);

				bitmap_line(30, dis_y_freq, 30, dis_y_freq + 60, 0xda); //0.1Hz
				bitmap_line(64, dis_y_freq, 64, dis_y_freq + 60, 0xda); //1Hz
				bitmap_line(98, dis_y_freq, 98, dis_y_freq + 60, 0xda); //10Hz
				bitmap_line(132, dis_y_freq, 132, dis_y_freq + 60, 0xda); //100Hz
				bitmap_line(166, dis_y_freq, 166, dis_y_freq + 60, 0xda); //1000Hz
				bitmap_line(200, dis_y_freq, 200, dis_y_freq + 60, 0xda); //10000Hz
				bitmap_stringBitmap(30, dis_y_freq + 62, "0.1", 0, 0, 0xda);
				bitmap_stringBitmap(64, dis_y_freq + 62, "1", 0, 0, 0xda);
				bitmap_stringBitmap(98, dis_y_freq + 62, "10", 0, 0, 0xda);
				bitmap_stringBitmap(132, dis_y_freq + 62, "100", 0, 0, 0xda);
				bitmap_stringBitmap(166, dis_y_freq + 62, "1k", 0, 0, 0xda);
				bitmap_stringBitmap(200, dis_y_freq + 62, "10k", 0, 0, 0xda);
				//bitmap_line(210, dis_y_freq, 210, dis_y_freq + 60, 0xda); //20000Hz

				float32_t axis_tmp;

				//vib
				dis_width_lfo_vib = (180.0f * control_value[VIB_AMP]) / 34579.0f;

				axis_tmp = 12.0f * log2f((20.0f * (float32_t)control_value[VIB_LFO] / 65535.0f) / 440.0f) + 69 + 76.2394537;
				axis_tmp = 30.0f + 180.0f * axis_tmp / 211.0f;
				dis_x_freq = (axis_tmp < 20) ? (20) : (uint16_t)(axis_tmp);
				if (20 < dis_x_freq) {
					bitmap_line(dis_x_freq, dis_y_freq, dis_x_freq, dis_y_freq + 60, color_behavior[SYNTH_IND_VIB]);
				}else{
					bitmap_line(dis_x_freq, dis_y_freq, dis_x_freq, dis_y_freq + 20, color_behavior[SYNTH_IND_VIB]);
				}

				//tre
				dis_width_lfo_tre = 20.0f + (200.0f * control_value[TRE_AMP]) / 34579.0f;
				bitmap_line(dis_width_lfo_tre, dis_y_amp, dis_width_lfo_tre, dis_y_amp + 60, color_behavior[SYNTH_IND_TRE]);

				axis_tmp = 12 * log2f((20.0f * (float32_t)control_value[TRE_LFO] / 65535.0f) / 440.0f) + 69.0f + 76.2394537f;
				axis_tmp = 30 + 180 * axis_tmp / 211;
				dis_x_freq = (axis_tmp < 20) ? (20) : (uint16_t)(axis_tmp);
				if (20 < dis_x_freq) {
					bitmap_line(dis_x_freq, dis_y_freq, dis_x_freq, dis_y_freq + 60, color_behavior[SYNTH_IND_TRE]);
				}else{
					bitmap_line(dis_x_freq, dis_y_freq + 21, dis_x_freq, dis_y_freq + 40, color_behavior[SYNTH_IND_TRE]);
				}

				//wow
				//filter
				axis_tmp = (control_value[LPF_CUTOFF] / 256.0f) + 76.2394537;
				axis_tmp = 30 + 180 * axis_tmp / 211;
				dis_x_freq = (axis_tmp < 20) ? (20) : (uint16_t)(axis_tmp);
				dis_width_lfo_wow = (180 * control_value[WOW_AMP]) / 34579;

				bitmap_line(dis_x_freq, dis_y_freq, dis_x_freq, dis_y_freq + 60, color_behavior[SYNTH_IND_FIL]);
				bitmap_line(dis_x_freq - dis_width_lfo_wow, dis_y_freq, dis_x_freq - dis_width_lfo_wow, dis_y_freq + 60, color_behavior[SYNTH_IND_FIL]);
				bitmap_line(dis_x_freq + dis_width_lfo_wow, dis_y_freq, dis_x_freq + dis_width_lfo_wow, dis_y_freq + 60, color_behavior[SYNTH_IND_FIL]);

				axis_tmp = 12.0f * log2f((20.0f * (float32_t)control_value[WOW_LFO] / 65535.0f) / 440.0f) + 69.0f + 76.2394537f;
				axis_tmp = 30.0f + 180.0f * axis_tmp / 211.0f;
				dis_x_freq = (axis_tmp < 20) ? (20) : (uint16_t)(axis_tmp);
				if (20 < dis_x_freq) {
					bitmap_line(dis_x_freq, dis_y_freq, dis_x_freq, dis_y_freq + 60, color_behavior[SYNTH_IND_WOW]);
				}else{
					bitmap_line(dis_x_freq, dis_y_freq + 41, dis_x_freq, dis_y_freq + 60, color_behavior[SYNTH_IND_WOW]);
				}

				//signal
				for(uint8_t chord = 0; chord < 10; chord++){
					if(control_value[S1_ENABLE + (6 * chord)]){
						bitmap_fillrect(14, dis_y_freq, 15, dis_y_freq + 4, color_behavior[SYNTH_IND_SIG]);
						bitmap_fillrect(14, dis_y_amp, 15, dis_y_amp + 4, color_behavior[SYNTH_IND_SIG]);
					}

					sprintf(string, "%2d", chord + 1);

					//freq
					bitmap_stringBitmap(5, dis_y_freq, string, 0, 0, 0xff);

					axis_tmp = (control_value[S1_FREQ + (6 * chord)] / 256.0f) + 76.2394537f;
					axis_tmp = 30 + 180 * axis_tmp / 211;
					dis_x_freq = (axis_tmp < 20) ? (20) : (uint16_t)(axis_tmp);
					bitmap_line(20, dis_y_freq + 2, 220, dis_y_freq + 2, 0xda);
					bitmap_line(dis_x_freq - dis_width_lfo_vib, dis_y_freq + 2, dis_x_freq + dis_width_lfo_vib, dis_y_freq + 2, color_behavior[SYNTH_IND_VIB]);
					bitmap_rect(dis_x_freq - 1, dis_y_freq + 1, dis_x_freq + 1, dis_y_freq + 3, 1, color_behavior[SYNTH_IND_SIG]);

					dis_y_freq += 6;

					//amp
					bitmap_stringBitmap(5, dis_y_amp, string, 0, 0, 0xff);
					bitmap_line(20, dis_y_amp + 2, 220, dis_y_amp + 2, 0xda);
					uint16_t sig_num_view;

					sig_num_view = (control_value[SYNC_WAVFORM] == 0)?(S1_SIN + (6 * chord)):(S1_SIN);
					if(control_value[sig_num_view] > 0){
						dis_x_amp = 20 + 200 * control_value[sig_num_view] / 512;
						if(dis_x_amp > 220){
							dis_x_amp = 220;
						}
						bitmap_rect(dis_x_amp - 1, dis_y_amp + 1, dis_x_amp + 1, dis_y_amp + 3, 1, color_signal[SYNTH_SIG_SIN]);
					}
					sig_num_view = (control_value[SYNC_WAVFORM] == 0)?(S1_SQU + (6 * chord)):(S1_SQU);
					if(control_value[sig_num_view] > 0){
						dis_x_amp = 20 + 200 * control_value[sig_num_view] / 512;
						if(dis_x_amp > 220){
							dis_x_amp = 220;
						}
						bitmap_rect(dis_x_amp - 1, dis_y_amp + 1, dis_x_amp + 1, dis_y_amp + 3, 1, color_signal[SYNTH_SIG_SQU]);
					}
					sig_num_view = (control_value[SYNC_WAVFORM] == 0)?(S1_SAW + (6 * chord)):(S1_SAW);
					if(control_value[sig_num_view] > 0){
						dis_x_amp = 20 + 200 * control_value[sig_num_view] / 512;
						if(dis_x_amp > 220){
							dis_x_amp = 220;
						}
						bitmap_rect(dis_x_amp - 1, dis_y_amp + 1, dis_x_amp + 1, dis_y_amp + 3, 1, color_signal[SYNTH_SIG_SAW1]);
					}
					sig_num_view = (control_value[SYNC_WAVFORM] == 0)?(S1_TRI + (6 * chord)):(S1_TRI);
					if(control_value[sig_num_view] > 0){
						dis_x_amp = 20 + 200 * control_value[sig_num_view] / 512;
						if(dis_x_amp > 220){
							dis_x_amp = 220;
						}
						bitmap_rect(dis_x_amp - 1, dis_y_amp + 1, dis_x_amp + 1, dis_y_amp + 3, 1, color_signal[SYNTH_SIG_TRI]);
					}

					dis_y_amp += 6;
				}

				displaySplitIndex = 0;
			}
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_SLAVE;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 100;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 20;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * Enable DMA controller clock
  * Configure DMA for memory to memory transfers
  *   hdma_memtomem_dma2_stream0
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma2_stream0 on DMA2_Stream0 */
  hdma_memtomem_dma2_stream0.Instance = DMA2_Stream0;
  hdma_memtomem_dma2_stream0.Init.Channel = DMA_CHANNEL_0;
  hdma_memtomem_dma2_stream0.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma2_stream0.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem_dma2_stream0.Init.MemInc = DMA_MINC_DISABLE;
  hdma_memtomem_dma2_stream0.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_memtomem_dma2_stream0.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_memtomem_dma2_stream0.Init.Mode = DMA_NORMAL;
  hdma_memtomem_dma2_stream0.Init.Priority = DMA_PRIORITY_MEDIUM;
  hdma_memtomem_dma2_stream0.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma_memtomem_dma2_stream0.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma_memtomem_dma2_stream0.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_memtomem_dma2_stream0.Init.PeriphBurst = DMA_PBURST_SINGLE;
  if (HAL_DMA_Init(&hdma_memtomem_dma2_stream0) != HAL_OK)
  {
    Error_Handler( );
  }

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : SPI1_SWNSS_Pin touch_it_Pin */
  GPIO_InitStruct.Pin = SPI1_SWNSS_Pin|touch_it_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

  /* USER CODE BEGIN FSMC_Init 0 */

  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 6;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 6;
  Timing.BusTurnAroundDuration = 0;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FSMC_Init 2 */
	Timing.AddressHoldTime = 0;
	Timing.CLKDivision = 0;
	Timing.DataLatency = 0;

  /* USER CODE END FSMC_Init 2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
