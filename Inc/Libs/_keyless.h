/*
 * _keyless.h
 *
 *  Created on: Mar 4, 2020
 *      Author: pudja
 */

#ifndef KEYLESS_H_
#define KEYLESS_H_

#include "_nrf24l01.h"

typedef struct {
  uint8_t count;
  uint8_t rx[8];
} payload_t;

void KEYLESS_Init(void);
void KEYLESS_IrqHandler(void);
uint8_t KEYLESS_ReadPayload(void);

#endif /* KEYLESS_H_ */
