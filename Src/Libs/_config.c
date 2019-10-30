/*
 * _config.c
 *
 *  Created on: Aug 26, 2019
 *      Author: Puja
 */

#include "_config.h"

void BSP_Led_Write_All(uint8_t state) {
	HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, state);
	HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, state);
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, state);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, state);
}

void BSP_Led_Toggle_All(void) {
	HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
	HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
	HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}

void BSP_Led_Disco(uint16_t ms) {
	uint8_t i = 100;
	uint16_t delay = ms / i;

	while (i--) {
		BSP_Led_Toggle_All();
		osDelay(delay);
	}

}

// Converts a floating point number to string.
void ftoa(float f, char *str, char size) {
	uint8_t pos;  // position in string
	char len;  // length of decimal part of result
	char curr[100];  // temp holder for next digit
	int value;  // decimal digit(s) to convert
	pos = 0;  // initialize pos, just to be sure

	value = (int) f;  // truncate the floating point number
	itoa(value, str, 10);  // this is kinda dangerous depending on the length of str
	// now str array has the digits before the decimal

	if (f < 0)  // handle negative numbers
			{
		f *= -1;
		value *= -1;
	}

	len = strlen(str);  // find out how big the integer part was
	pos = len;  // position the pointer to the end of the integer part
	str[pos++] = '.';  // add decimal point to string

	while (pos < (size + len + 1))  // process remaining digits
	{
		f = f - (float) value;  // hack off the whole part of the number
		f *= 10;  // move next digit over
		value = (int) f;  // get next digit
		itoa(value, curr, 10); // convert digit to string
		str[pos++] = *curr; // add digit to result string and increment pointer
	}
}