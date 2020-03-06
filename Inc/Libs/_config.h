/*
 * _config.h
 *
 *  Created on: Aug 26, 2019
 *      Author: Puja
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string.h>								// for: strlen()
#include <stdlib.h>								// for: itoa()
#include <stdio.h>								// for: sprintf()
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "main.h"
#include "cmsis_os.h"
#include "_swv.h"
#include "_database.h"

// Function prototype
void _LedWrite(uint8_t state);
void _LedToggle(void);
void _LedDisco(uint16_t ms);
uint8_t _TimeNeedCalibration(rtc_t rtc);
uint8_t _TimeCheckDaylight(timestamp_t timestamp);
int8_t _BitPosition(uint64_t event_id);
int32_t _ParseNumber(const char *ptr, uint8_t *cnt);
float _ParseFloatNumber(const char *ptr, uint8_t *cnt);

#endif /* CONFIG_H_ */
