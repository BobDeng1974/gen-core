/*
 * _DMA_Finger.c
 *
 *  Created on: Aug 27, 2019
 *      Author: Puja
 */

#include "_DMA_Finger.h"

extern DMA_HandleTypeDef hdma_uart4_rx;
extern UART_HandleTypeDef huart4;

char FINGER_DMA_RX_Buffer[FINGER_DMA_RX_BUFFER_SIZE];
char FINGER_UART_RX_Buffer[FINGER_UART_RX_BUFFER_SIZE];
size_t finger_write, finger_len, finger_tocopy;
uint8_t *finger_ptr;

void FINGER_USART_IrqHandler(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma) {
	if (huart->Instance->SR & UART_FLAG_IDLE) /* if Idle flag is set */
	{
		__HAL_UART_CLEAR_IDLEFLAG(huart); /* Clear idle flag */
		__HAL_DMA_DISABLE(hdma); /* Disabling DMA will force transfer complete interrupt if enabled */
		FINGER_DMA_IrqHandler(hdma, huart);
	}
}

void FINGER_DMA_IrqHandler(DMA_HandleTypeDef *hdma, UART_HandleTypeDef *huart) {
	if (__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_TC) != RESET) // if the source is TC
			{
		/* Clear the transfer complete flag */
		__HAL_DMA_CLEAR_FLAG(hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));

		/* Get the finger_length of the data */
		finger_len = FINGER_DMA_RX_BUFFER_SIZE - hdma->Instance->NDTR;
		/* Only process if DMA is not empty */
		if (finger_len > 0) {
			/* Reset the buffer */
			FINGER_Reset_Buffer();
			/* Get number of bytes we can copy to the end of buffer */
			finger_tocopy = FINGER_UART_RX_BUFFER_SIZE - finger_write;
			/* finger_write received data for UART main buffer for manipulation later */
			finger_ptr = (uint8_t *) FINGER_DMA_RX_Buffer;
			/* Check how many bytes to copy */
			if (finger_tocopy > finger_len) {
				finger_tocopy = finger_len;
			}
			/* Copy first part */
			memcpy(&FINGER_UART_RX_Buffer[finger_write], finger_ptr, finger_tocopy);
			/* Correct values for remaining data */
			finger_write += finger_tocopy;
			finger_len -= finger_tocopy;
			finger_ptr += finger_tocopy;

			/* If still data to finger_write for beginning of buffer */
			if (finger_len) {
				/* Don't care if we override Read pointer now */
				memcpy(&FINGER_UART_RX_Buffer[0], finger_ptr, finger_len);
				finger_write = finger_len;
			}
			// set null at the end
			//			FINGER_UART_RX_Buffer[finger_write] = '\0';
		}

		/* Start DMA transfer again */
		hdma->Instance->CR |= DMA_SxCR_EN;
	}
}

void FINGER_DMA_Init(void) {
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);      // enable idle line interrupt
	__HAL_DMA_ENABLE_IT(&hdma_uart4_rx, DMA_IT_TC);  // enable DMA Tx cplt interrupt
	__HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT); // disable half complete interrupt
	HAL_UART_Receive_DMA(&huart4, (uint8_t *) FINGER_DMA_RX_Buffer, FINGER_DMA_RX_BUFFER_SIZE);
}

void FINGER_Reset_Buffer(void) {
	// clear rx buffer
	memset(FINGER_UART_RX_Buffer, 0, sizeof(FINGER_UART_RX_Buffer));
	// set index back to first
	finger_write = 0;
}