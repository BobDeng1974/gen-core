/*
 * _rtc.h
 *
 *  Created on: Oct 9, 2019
 *      Author: Puja
 */

#ifndef RTC_H_
#define RTC_H_

#include "main.h"
#include "_config.h"

typedef struct {
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
} timestamp_t;

void RTC_Read_RAW(timestamp_t *timestamp);
uint64_t RTC_Read(void);
void RTC_Write(uint64_t dateTime);
//uint8_t RTC_Offset(uint8_t hour, int offset);

#endif /* RTC_H_ */
