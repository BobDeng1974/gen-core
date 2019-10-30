/*
 * DMA_Ublox.c
 *
 *  Created on: Aug 14, 2019
 *      Author: Puja
 */

#include "_DMA_Ublox.h"

extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart2;

char UBLOX_DMA_RX_Buffer[UBLOX_DMA_RX_BUFFER_SIZE];
char UBLOX_UART_RX_Buffer[UBLOX_UART_RX_BUFFER_SIZE];
size_t ublox_write, ublox_len, ublox_tocopy;
uint8_t *ublox_ptr;

void UBLOX_USART_IrqHandler(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma) {
	if (huart->Instance->SR & UART_FLAG_IDLE) /* if Idle flag is set */
	{
		__HAL_UART_CLEAR_IDLEFLAG(huart); /* Clear idle flag */
		__HAL_DMA_DISABLE(hdma); /* Disabling DMA will force transfer complete interrupt if enabled */
		UBLOX_DMA_IrqHandler(hdma, huart);
	}
}

void UBLOX_DMA_IrqHandler(DMA_HandleTypeDef *hdma, UART_HandleTypeDef *huart) {
	if (__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_TC) != RESET) { // if the source is TC

		/* Clear the transfer complete flag */
		__HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));

		/* Get the ublox_length of the data */
		ublox_len = UBLOX_DMA_RX_BUFFER_SIZE - hdma->Instance->NDTR;
		/* Only process if DMA is not empty */
		if (ublox_len > 0) {
			/* Reset the buffer */
			UBLOX_Reset_Buffer();
			/* Get number of bytes we can copy to the end of buffer */
			ublox_tocopy = UBLOX_UART_RX_BUFFER_SIZE - ublox_write;
			/* ublox_write received data for UART main buffer for manipulation later */
			ublox_ptr = (uint8_t *) UBLOX_DMA_RX_Buffer;
			/* Check how many bytes to copy */
			if (ublox_tocopy > ublox_len) {
				ublox_tocopy = ublox_len;
			}
			/* Copy first part */
			memcpy(&UBLOX_UART_RX_Buffer[ublox_write], ublox_ptr, ublox_tocopy);
			/* Correct values for remaining data */
			ublox_write += ublox_tocopy;
			ublox_len -= ublox_tocopy;
			ublox_ptr += ublox_tocopy;

			/* If still data to ublox_write for beginning of buffer */
			if (ublox_len) {
				/* Don't care if we override Read pointer now */
				memcpy(&UBLOX_UART_RX_Buffer[0], ublox_ptr, ublox_len);
				ublox_write = ublox_len;
			}
			// set null at the end
			UBLOX_UART_RX_Buffer[ublox_write] = '\0';
		}

		/* Start DMA transfer again */
		hdma->Instance->CR |= DMA_SxCR_EN;
	}
}

void UBLOX_DMA_Init(void) {
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);      // enable idle line interrupt
	__HAL_DMA_ENABLE_IT(&hdma_usart2_rx, DMA_IT_TC);  // enable DMA Tx cplt interrupt
	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT); // disable half complete interrupt
	HAL_UART_Receive_DMA(&huart2, (uint8_t *) UBLOX_DMA_RX_Buffer, UBLOX_DMA_RX_BUFFER_SIZE);
}

void UBLOX_Reset_Buffer(void) {
	// clear rx buffer
	//	memset(UBLOX_UART_RX_Buffer, 0, sizeof(UBLOX_UART_RX_Buffer));
	// set index back to first
	ublox_write = 0;
	//	 set null at the end
	UBLOX_UART_RX_Buffer[ublox_write] = '\0';
}