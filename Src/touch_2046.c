/*
 * touch_2046.c
 *
 *  Created on: Jun 5, 2020
 *      Author: omurakosuke
 */

#include <touch_2046.h>

Coordinate_t xpt2046_read(SPI_HandleTypeDef *_hspi, Touch_calib_t _calib) {
	Coordinate_t output;

	uint8_t raw[2] = { 0 };
	uint8_t zero[2] = { 0 };
	uint8_t cmd[1] = { 0 };

	uint32_t x = 0;
	uint32_t y = 0;
	uint32_t z1 = 0;
	uint32_t z2 = 0;

	//dummy
	cmd[0] = READ_BASE | READ_X;
	HAL_SPI_Transmit(_hspi, cmd, 1, 1000);
	HAL_SPI_TransmitReceive(_hspi, zero, raw, 2, 1000);

	for (uint8_t i = 0; i < XPT2046_SAMPLE; i++) {
		/*table7
		 * 7:S:1>table16
		 * 6-4:A:>table1&16
		 * 3:MODE:0
		 * 2:S/D>table1&2
		 * 1-0:PD:>table5
		 * */

		cmd[0] = READ_BASE | READ_Y;
		HAL_SPI_Transmit(_hspi, cmd, 1, 1000);
		HAL_SPI_TransmitReceive(_hspi, zero, raw, 2, 1000);
		y += (((uint16_t) raw[0]) << 5) | ((uint16_t) raw[1] >> 3);

		cmd[0] = READ_BASE | READ_X;
		HAL_SPI_Transmit(_hspi, cmd, 1, 1000);
		HAL_SPI_TransmitReceive(_hspi, zero, raw, 2, 1000);
		x += (((uint16_t) raw[0]) << 5) | ((uint16_t) raw[1] >> 3);

		cmd[0] = READ_BASE | READ_Z1;
		HAL_SPI_Transmit(_hspi, cmd, 1, 1000);
		HAL_SPI_TransmitReceive(_hspi, zero, raw, 2, 1000);
		z1 += (((uint16_t) raw[0]) << 5) | ((uint16_t) raw[1] >> 3);

		cmd[0] = READ_BASE | READ_Z2;
		HAL_SPI_Transmit(_hspi, cmd, 1, 1000);
		HAL_SPI_TransmitReceive(_hspi, zero, raw, 2, 1000);
		z2 += (((uint16_t) raw[0]) << 5) | ((uint16_t) raw[1] >> 3);
	}

	x /= XPT2046_SAMPLE;
	y /= XPT2046_SAMPLE;
	z1 /= XPT2046_SAMPLE;
	z2 /= XPT2046_SAMPLE;

	switch (XPT2046_ROTATION) {
	case 0:
	default:
		break;
	case 1:
		x = XPT2046_MAX - x;
		y = XPT2046_MAX - y;
		break;
	case 2:
		y = XPT2046_MAX - y;
		break;
	case 3:
		x = XPT2046_MAX - x;
		break;
	}

	//to calibrate comment-out here
	if (x < _calib.minX) {
		x = 0;
	} else if (x > _calib.maxX) {
		x = XPT2046_WIDTH;
	} else {
		x -= _calib.minX;
		x = (uint16_t) (XPT2046_WIDTH * (float)x / (_calib.maxX - _calib.minX));
	}

	if (y < _calib.minY) {
		y = 0;
	} else if (y > _calib.maxY) {
		y = XPT2046_WIDTH;
	} else {
		y -= _calib.minY;
		y = (uint16_t) (XPT2046_HEIGHT * (float)y / (_calib.maxY - _calib.minY));
	}
	//to calibrate comment-out here

	output.x = (uint16_t) x;
	output.y = (uint16_t) y;
	output.z = (uint16_t) ((10000 * z1) / z2);

	return output;
}

