/*
 * _rtc.h
 *
 *  Created on: Oct 9, 2019
 *      Author: Puja
 */

#ifndef RTC_H_
#define RTC_H_

#include "_utils.h"

timestamp_t RTC_Decode(uint64_t dateTime);
uint64_t RTC_Encode(timestamp_t timestamp);
uint64_t RTC_Read(void);
void RTC_Read_RAW(timestamp_t *timestamp);
void RTC_Write(uint64_t dateTime, rtc_t *rtc);
void RTC_Write_RAW(timestamp_t *timestamp, rtc_t *rtc);

#endif /* RTC_H_ */
