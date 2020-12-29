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

#include <FSMC_ILI9341.h>
#include <elementary_wave.h>
#include <bitmap.h>
#include <touch_2046.h>
#include <synthesizer_parameters.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef union {
	uint16_t raw;
	struct {
		unsigned int high :8;
		unsigned int low :8;
	} split;
} transmit16;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define XPT2046_MIN_RAW_X 230
#define XPT2046_MAX_RAW_X 3900
#define XPT2046_MIN_RAW_Y 290
#define XPT2046_MAX_RAW_Y 3870

#define STATUS_WAIT 0
#define STATUS_HALF 1
#define STATUS_DONE 2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi3_rx;

TIM_HandleTypeDef htim6;

DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */
uint8_t status = STATUS_DONE;

//uint8_t spi_controler_tx[4] = {0};
uint8_t spi_controler_rx[4];
uint16_t control_value[SIZE_PARAMATER] = { 0 };

uint8_t touch_it = 0;
uint8_t btn_it = 0;
uint16_t spi_it = 0;
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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
	case GPIO_PIN_3:
	case GPIO_PIN_4:
		if (btn_it == 0) {
			btn_it = GPIO_Pin;
		}
		break;
	case GPIO_PIN_5:
		touch_it++;
		break;
	}
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
	transmit16 data;
	data.split.low = spi_controler_rx[0];
	data.split.high = spi_controler_rx[1];
	uint16_t parameter = data.raw;

	data.split.low = spi_controler_rx[2];
	data.split.high = spi_controler_rx[3];
	control_value[parameter] = data.raw;

	HAL_SPI_Receive_IT(&hspi3, spi_controler_rx, 4);

	spi_it = parameter + 1;
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
	//HAL_GPIO_WritePin(GPIOA, Board_D2_Pin, GPIO_PIN_SET);
	status = STATUS_DONE;
}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
	//HAL_GPIO_WritePin(GPIOA, Board_D2_Pin, GPIO_PIN_RESET);
	status = STATUS_HALF;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	/* USER CODE BEGIN 2 */
	ILI9341_init();
	ILI9341_setRotation(3);
	ILI9341_fill_all(TFT_ORANGE);

	//xpt2046 init
	//xpt2046_init(&hspi2);

	char string[20];

	HAL_GPIO_WritePin(GPIOA, Board_D2_Pin, GPIO_PIN_SET);	//status
	sprintf(string, "STM32F407");
	ILI9341_addString(80, 155, string, TFT_CYAN, 1);
	sprintf(string, "Synthesizer");
	ILI9341_addString(70, 170, string, TFT_GREENYELLOW, 1);

	HAL_Delay(2000);
	ILI9341_fill_all(TFT_BLACK);
	HAL_GPIO_WritePin(GPIOA, Board_D2_Pin, GPIO_PIN_RESET);	//status

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	uint32_t audio_time_index = 0;
	uint16_t audio_dac[NUM_SAMPLING];
	float audio_in[SAMPLE_ALL] = { 0 };
	float audio_out[SAMPLE_ALL] = { 0 };
	//float signal_freq = 0;

	arm_biquad_cascade_df2T_instance_f32 S;
	float32_t pCoeffs[5];
	float32_t buffer[2];

	uint16_t amp_characteristic[ILI9341_WIDTH];
	float cutoff_freq = 0;
	float q_factor = 0.7;
	uint16_t prev_cutoff_NN = 0xffff;
	float prev_q_factor = 100;
	uint16_t freq_1 = 0;

	uint8_t filter_changed = 0;

	ColorUnion background;
	ColorUnion plot1;
	ColorUnion plot2;
	ColorUnion bar;

	uint16_t area_height_top = 10;
	uint16_t area_height_wave = 150;
	//uint16_t log_axis[26] = { 15, 25, 33, 39, 44, 49, 53, 57, 81, 95, 105, 112, 119, 124, 129, 133, 136, 161, 174, 184, 192, 198, 204, 208, 212, 216 };

	background.raw = TFT_BLACK;
	plot1.raw = TFT_yellow;
	plot2.raw = TFT_blue;
	bar.raw = miner_white;

	Coordinate_t touch;
	Touch_calib_t touch_cal;
	//calibration value
	touch_cal.minX = XPT2046_MIN_RAW_X;
	touch_cal.minY = XPT2046_MIN_RAW_Y;
	touch_cal.maxX = XPT2046_MAX_RAW_X;
	touch_cal.maxY = XPT2046_MAX_RAW_Y;

	//Frequency axis of amplitude characteristics
	sprintf(string, "100");
	ILI9341_string(52, 311, string, TFT_WHITE, background.raw, 0);
	sprintf(string, "1000");
	ILI9341_string(130, 311, string, TFT_WHITE, background.raw, 0);
	sprintf(string, "10000");
	ILI9341_string(205, 311, string, TFT_WHITE, background.raw, 0);

	//start timer and DAC
	HAL_TIM_GenerateEvent(&htim6, TIM_EVENTSOURCE_UPDATE);
	HAL_TIM_Base_Start(&htim6);
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) audio_dac,
	NUM_SAMPLING, DAC_ALIGN_12B_R);

	//input spi
	HAL_SPI_Receive_IT(&hspi3, spi_controler_rx, 4);

	//init value(for test)
	///*
	control_value[S1_ENABLE] = 1;
	control_value[S1_FREQ] = 17664; //440Hz
	control_value[S1_SIN] = 100;
	control_value[S1_SQU] = 0;
	control_value[S1_TRI] = 0;
	control_value[S1_SAW] = 0;
	control_value[LPF_CUTOFF] = 24320; //2kHz
	control_value[LPF_Q] = 0.7 * 100;
	//*/

	while (1) {
		//input touch
		touch = xpt2046_read(&hspi2, touch_cal);
		if ((touch.z > 50)/* || !HAL_GPIO_ReadPin(GPIOC, touch_it_Pin)*/) {
			if (touch.y < 150) {
				control_value[LPF_CUTOFF] = 150 * touch.x;
				control_value[LPF_Q] = 40 + (2 * touch.y / 3);
			} else if (touch.y < 180) {
			} else if (touch.y < 300) {
				control_value[S1_FREQ] = 144 * (239 - touch.x);
				uint16_t amp = ((touch.y - 180) * 2048) / 120;
				control_value[S1_SIN] = amp;
			} else if (touch.y < ILI9341_HEIGHT) {
			}

			touch_it = 0;
		}

		//transform value
		q_factor = control_value[LPF_Q] / (float) 100;

		switch (status) {
		case STATUS_HALF:
			//update array for DAC(First half)
			float2uint(audio_out, audio_dac, 0, HALF_NUM_SAMPLING);

			//display audio wave
			//Search for an appropriate point to display on the LCD
			uint16_t LCD_start_index = PRE_SAMPLE;
			for (uint16_t i = (PRE_SAMPLE + 1); i < (SAMPLE_ALL - 320); i++) {
				if ((audio_in[i - 1] < GND_LEVEL)
						&& (audio_in[i] >= GND_LEVEL)) {
					LCD_start_index = i;
					break;
				}
			}

			//display information
			sprintf(string, "%5d[Hz]", freq_1);
			ILI9341_string(168, 1, string, TFT_WHITE, background.raw, 1);

			//display wave
			ILI9341_setRect(0, area_height_top, ILI9341_WIDTH - 1,
					area_height_top + area_height_wave - 1);
			for (uint16_t y = 0; y < area_height_wave; y++) {
				uint16_t t = 0;
				for (uint16_t x = 0; x < ILI9341_WIDTH; x++) {
					//scaling for amplitude
					uint16_t amp_scaled1 = (area_height_wave
							* (uint16_t) (audio_out[t + LCD_start_index]))
							/ 4096;
					amp_scaled1 = (area_height_wave - 1) - amp_scaled1;

					uint16_t amp_scaled2 = (area_height_wave
							* (uint16_t) (audio_in[t + LCD_start_index]))
							/ 4096;
					amp_scaled2 = (area_height_wave - 1) - amp_scaled2;
					t++;

					//display
					if (y == amp_scaled1) {
						ILI9341_sendData16(plot1.raw);
					} else if (y == amp_scaled2) {
						ILI9341_sendData16(plot2.raw);
					} else if (y == 75) {
						ILI9341_sendData16(bar.raw);
					} else {
						ILI9341_sendData16(background.raw);
					}
				}
			}

			//display amplitude characteristic of filter
			if (filter_changed) {
				filter_changed = 0;
				sprintf(string, "Q %d.%02d[-]", (uint8_t) q_factor,
						(uint8_t) (q_factor * 100) % 100);
				ILI9341_string(50, 161, string, TFT_WHITE, background.raw, 1);
				sprintf(string, "fc %5d[Hz]", (uint16_t) cutoff_freq);
				ILI9341_string(144, 161, string, TFT_WHITE, background.raw, 1);

				ILI9341_setRect(0, 2 * area_height_top + area_height_wave,
				ILI9341_WIDTH - 1, 310 - 1);
				for (uint16_t y = 0; y < (area_height_wave - 10); y++) {
					for (uint16_t x = 0; x < ILI9341_WIDTH; x++) {
						if (y == amp_characteristic[x]) {
							ILI9341_sendData16(plot1.raw);
						} else if (y == 20) {
							ILI9341_sendData16(bar.raw);
						} else {
							switch (x) {
							case 15:		//30
							case 25:		//40
							case 33:		//50
							case 39:		//60
							case 44:		//70
							case 49:		//80
							case 53:		//90
							case 57:		//100
							case 81:		//200
							case 95:		//300
							case 105:		//400
							case 112:		//500
							case 119:		//600
							case 124:		//700
							case 129:		//800
							case 133:		//900
							case 136:		//1000
							case 161:		//2000
							case 174:		//3000
							case 184:		//4000
							case 192:		//5000
							case 198:		//6000
							case 204:		//7000
							case 208:		//8000
							case 212:		//9000
							case 216:		//10000
								ILI9341_sendData16(bar.raw);
								break;
							default:
								ILI9341_sendData16(background.raw);
								break;
							}
						}
					}
				}
			}

			//Processing Exit
			status = STATUS_WAIT;
			break;
		case STATUS_DONE:
			HAL_GPIO_WritePin(GPIOA, Board_D2_Pin, GPIO_PIN_SET); //status calc

			//create filter(If the q-factor or cutoff-frequency is changed)
			if (control_value[LPF_CUTOFF] > MAX_FREQ_NOTE) {
				control_value[LPF_CUTOFF] = MAX_FREQ_NOTE;
			}
			if ((prev_cutoff_NN != control_value[LPF_CUTOFF])
					|| (prev_q_factor != q_factor)) {

				cutoff_freq = 440
						* pow(2,
								((float) control_value[LPF_CUTOFF] / 3072)
										- 5.75);

				calc_lpf_coeffs(pCoeffs, cutoff_freq, q_factor);
				arm_biquad_cascade_df2T_init_f32(&S, 1, pCoeffs, buffer);

				calc_amp_char(amp_characteristic, cutoff_freq, q_factor);
				filter_changed++;

				//
				prev_q_factor = q_factor;
				prev_cutoff_NN = control_value[LPF_CUTOFF];
			}

			//update array for DAC(Latter half)
			float2uint(audio_out, audio_dac, HALF_NUM_SAMPLING, NUM_SAMPLING);

			//audio array initialize
			arm_fill_f32(0, (audio_in + PRE_SAMPLE), NUM_SAMPLING);

			//audio create
			uint16_t signal_set_index = S1_ENABLE;
			for (uint8_t chord = 0; chord < 10; chord++) {
				//check signal_x enable/disable
				if (0 == control_value[signal_set_index++]) {
					signal_set_index += 5;
					continue;
				}

				//convert Notenum to freq.
				float freq = 440
						* pow(2,
								((float) control_value[signal_set_index++]
										/ 3072) - 5.75);
				if (freq > MAX_FREQ) {
					freq = MAX_FREQ;
				}

				if (chord == 0) {
					freq_1 = (uint16_t) freq;
				}

				float period_us = 1000000 / freq;

				//create each wave
				add_sin(audio_in, audio_time_index, freq,
						(uint16_t) control_value[signal_set_index++],
						NUM_SAMPLING);
				add_square(audio_in, audio_time_index, period_us,
						(uint16_t) control_value[signal_set_index++],
						NUM_SAMPLING);
				add_sawtooth(audio_in, audio_time_index, period_us,
						(uint16_t) control_value[signal_set_index++],
						NUM_SAMPLING);
				add_triangle(audio_in, audio_time_index, period_us,
						(uint16_t) control_value[signal_set_index++],
						NUM_SAMPLING);
			}

			//If the index value becomes too large, waveform generation will not be successful.
			//4194304 = 2^22, 104.8sec
			audio_time_index =
					(audio_time_index > 4194304) ?
							(0) : (audio_time_index + NUM_SAMPLING);

			//input filter
			arm_biquad_cascade_df2T_f32(&S, audio_in, audio_out, SAMPLE_ALL);

			//pre-sample for filter (for next cycle)
			arm_copy_f32((audio_in + NUM_SAMPLING), audio_in, PRE_SAMPLE);

			//offset audio GND
			arm_offset_f32((audio_in + PRE_SAMPLE), GND_LEVEL,
					(audio_in + PRE_SAMPLE), NUM_SAMPLING);
			arm_offset_f32((audio_out + PRE_SAMPLE), GND_LEVEL,
					(audio_out + PRE_SAMPLE), NUM_SAMPLING);

			//Processing Exit
			HAL_GPIO_WritePin(GPIOA, Board_D2_Pin, GPIO_PIN_RESET); //status calc end
			status = STATUS_WAIT;
			break;
		case STATUS_WAIT:
		default:
			break;
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
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief DAC Initialization Function
 * @param None
 * @retval None
 */
static void MX_DAC_Init(void) {

	/* USER CODE BEGIN DAC_Init 0 */

	/* USER CODE END DAC_Init 0 */

	DAC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN DAC_Init 1 */

	/* USER CODE END DAC_Init 1 */
	/** DAC Initialization
	 */
	hdac.Instance = DAC;
	if (HAL_DAC_Init(&hdac) != HAL_OK) {
		Error_Handler();
	}
	/** DAC channel OUT1 config
	 */
	sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN DAC_Init 2 */

	/* USER CODE END DAC_Init 2 */

}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void) {

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
	if (HAL_SPI_Init(&hspi2) != HAL_OK) {
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
static void MX_SPI3_Init(void) {

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
	if (HAL_SPI_Init(&hspi3) != HAL_OK) {
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
static void MX_TIM6_Init(void) {

	/* USER CODE BEGIN TIM6_Init 0 */

	/* USER CODE END TIM6_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM6_Init 1 */

	/* USER CODE END TIM6_Init 1 */
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 100;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 20;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig)
			!= HAL_OK) {
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
static void MX_DMA_Init(void) {

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
	if (HAL_DMA_Init(&hdma_memtomem_dma2_stream0) != HAL_OK) {
		Error_Handler();
	}

	/* DMA interrupt init */
	/* DMA1_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
	/* DMA1_Stream5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(Board_D2_GPIO_Port, Board_D2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : btn_k1_Pin btn_k0_Pin */
	GPIO_InitStruct.Pin = btn_k1_Pin | btn_k0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : Board_D2_Pin */
	GPIO_InitStruct.Pin = Board_D2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(Board_D2_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : touch_it_Pin */
	GPIO_InitStruct.Pin = touch_it_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(touch_it_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void) {

	/* USER CODE BEGIN FSMC_Init 0 */

	/* USER CODE END FSMC_Init 0 */

	FSMC_NORSRAM_TimingTypeDef Timing = { 0 };

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

	if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK) {
		Error_Handler();
	}

	/* USER CODE BEGIN FSMC_Init 2 */
	Timing.AddressHoldTime = 0;
	Timing.CLKDivision = 0;
	Timing.DataLatency = 0;

	/* USER CODE END FSMC_Init 2 */
}

/* USER CODE BEGIN 4 */

const uint16_t log_axis_f[] = { 20, 21, 21, 22, 22, 23, 24, 24, 25, 26, 27, 27,
		28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 44, 45, 46,
		48, 49, 50, 52, 53, 55, 57, 58, 60, 62, 64, 65, 67, 69, 71, 73, 76, 78,
		80, 82, 85, 87, 90, 93, 95, 98, 101, 104, 107, 110, 113, 117, 120, 124,
		127, 131, 135, 139, 143, 147, 151, 156, 160, 165, 170, 175, 180, 185,
		191, 196, 202, 208, 214, 220, 227, 233, 240, 247, 254, 262, 270, 278,
		286, 294, 303, 312, 321, 330, 340, 350, 360, 371, 381, 393, 404, 416,
		428, 441, 454, 467, 481, 495, 509, 524, 540, 555, 572, 588, 606, 623,
		642, 660, 680, 700, 720, 741, 763, 786, 809, 832, 857, 882, 908, 934,
		962, 990, 1019, 1049, 1080, 1111, 1144, 1177, 1212, 1247, 1284, 1322,
		1360, 1400, 1441, 1484, 1527, 1572, 1618, 1665, 1714, 1765, 1816, 1870,
		1924, 1981, 2039, 2099, 2160, 2224, 2289, 2356, 2425, 2496, 2569, 2645,
		2722, 2802, 2884, 2969, 3056, 3145, 3238, 3333, 3430, 3531, 3634, 3741,
		3851, 3964, 4080, 4200, 4323, 4449, 4580, 4714, 4853, 4995, 5141, 5292,
		5447, 5607, 5771, 5941, 6115, 6294, 6479, 6669, 6864, 7066, 7273, 7486,
		7706, 7932, 8164, 8404, 8650, 8904, 9165, 9433, 9710, 9995, 10288,
		10590, 10900, 11220, 11549, 11887, 12236, 12595, 12964, 13344, 13736,
		14138, 14553, 14980, 15419, 15871, 16337, 16816, 17309, 17816, 18339,
		18877, 19430, 20000 };
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
