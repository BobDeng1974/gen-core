/*
 * DMA_Simcom.c
 *
 *  Created on: Aug 14, 2019
 *      Author: Puja
 */

#include "_dma_simcom.h"

extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;

char SIMCOM_UART_RX_Buffer[SIMCOM_UART_RX_BUFFER_SIZE];
static char SIMCOM_DMA_RX_Buffer[SIMCOM_DMA_RX_BUFFER_SIZE];
static size_t write, len, copy;
static uint8_t *ptr;

void SIMCOM_USART_IrqHandler(void) {
  if (huart1.Instance->SR & UART_FLAG_IDLE) /* if Idle flag is set */
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart1); /* Clear idle flag */
    __HAL_DMA_DISABLE(&hdma_usart1_rx); /* Disabling DMA will force transfer complete interrupt if enabled */
    SIMCOM_DMA_IrqHandler();
  }
}

void SIMCOM_DMA_IrqHandler(void) {
  if (__HAL_DMA_GET_IT_SOURCE(&hdma_usart1_rx, DMA_IT_TC) != RESET) { // if the source is TC
    /* Clear the transfer complete flag */
    __HAL_DMA_CLEAR_FLAG(&hdma_usart1_rx, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_usart1_rx));
    /* Get the length of the data */
    len = SIMCOM_DMA_RX_BUFFER_SIZE - hdma_usart1_rx.Instance->NDTR;

    /* Only process if DMA is not empty */
    if (len > 0) {
      /* Get number of bytes we can copy to the end of buffer */
      copy = SIMCOM_UART_RX_BUFFER_SIZE - write;
      /* write received data for UART main buffer for manipulation later */
      ptr = (uint8_t*) SIMCOM_DMA_RX_Buffer;
      /* Check how many bytes to copy */
      if (copy > len) {
        copy = len;
      }
      /* Copy first part */
      memcpy(&SIMCOM_UART_RX_Buffer[write], ptr, copy);
      /* Correct values for remaining data */
      write += copy;
      len -= copy;
      ptr += copy;
      /* If still data to write for beginning of buffer */
      if (len) {
        /* Don't care if we override Read pointer now */
        memcpy(&SIMCOM_UART_RX_Buffer[0], ptr, len);
        write = len;
      }
      // set null at the end
      SIMCOM_UART_RX_Buffer[write] = '\0';
    }

    /* Start DMA transfer again */
    hdma_usart1_rx.Instance->CR |= DMA_SxCR_EN;
  }
}

void SIMCOM_DMA_Init(void) {
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);      // enable idle line interrupt
  __HAL_DMA_ENABLE_IT(&hdma_usart1_rx, DMA_IT_TC);  // enable DMA Tx cplt interrupt
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT); // disable half complete interrupt
  HAL_UART_Receive_DMA(&huart1, (uint8_t*) SIMCOM_DMA_RX_Buffer, SIMCOM_DMA_RX_BUFFER_SIZE);
}

void SIMCOM_Reset_Buffer(void) {
  // clear rx buffer
  memset(SIMCOM_UART_RX_Buffer, 0, strlen(SIMCOM_UART_RX_Buffer));
  // wail until clear is done
  osDelay(50);
  // set index back to first
  write = 0;
}

void SIMCOM_Transmit(char *pData, uint16_t Size) {
  HAL_UART_Transmit(&huart1, (uint8_t*) pData, Size, HAL_MAX_DELAY);
}
