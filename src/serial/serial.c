/**
 * @file serial.c
 * @author Mohamed Hassanin
 * @brief A serial communication used by the bootloader (USART at this case)
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "stm32f4xx_hal.h"
#include "serial.h"
#include "port.h"
#include "circ_buffer.h"

void Error_Handler(void);

static UART_HandleTypeDef gUart1Handle;
static DMA_HandleTypeDef gDma2Handle;
static uint8_t gReceiveBuffer[RECEIVE_BUFFER_SIZE];
static CircBuff_t gsReceiveBuffer;
static void strTransmit(const char * str, uint8_t size);

void 
HAL_UART_MspInit(UART_HandleTypeDef *hsart)
{
  if(hsart->Instance==USART1)
    {
      __HAL_RCC_USART1_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      __HAL_RCC_DMA2_CLK_ENABLE();

      GPIO_InitTypeDef GPIO_InitStruct = {0};

      GPIO_InitStruct.Pin = USART1_TX_PIN | USART1_RX_PIN;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

      gDma2Handle.Instance = DMA2_Stream7;
      gDma2Handle.Init.Channel = DMA_CHANNEL_4;
      gDma2Handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
      gDma2Handle.Init.PeriphInc = DMA_PINC_DISABLE;
      gDma2Handle.Init.MemInc = DMA_MINC_ENABLE;
      gDma2Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
      gDma2Handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
      gDma2Handle.Init.Mode = DMA_NORMAL;
      gDma2Handle.Init.Priority = DMA_PRIORITY_LOW;
      gDma2Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
      if (HAL_DMA_Init(&gDma2Handle) != HAL_OK)
        {
          Error_Handler();
        }
      DMA2_Stream7->PAR = (uint32_t)&USART1->DR;

      __HAL_LINKDMA(&gUart1Handle,hdmatx,gDma2Handle);

      /* USART1 interrupt Init */
      HAL_NVIC_SetPriority(USART1_IRQn, TICK_INT_PRIORITY+1, 0);
      HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
}

void 
Serial_Init(void)
{
  gUart1Handle.Instance = USART1;

  gUart1Handle.Init.BaudRate = 9600;
  gUart1Handle.Init.WordLength = UART_WORDLENGTH_8B;
  gUart1Handle.Init.StopBits = UART_STOPBITS_1;
  gUart1Handle.Init.Parity = UART_PARITY_NONE;
  gUart1Handle.Init.Mode = UART_MODE_TX_RX;

  if (HAL_UART_Init(&gUart1Handle) != HAL_OK)
    {
      Error_Handler();
    }
  
  /* Enable DMA mode for transmitter */
  USART1->CR3 |= USART_CR3_DMAT;

  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(&gUart1Handle, UART_IT_RXNE);
  
  gsReceiveBuffer = CircBuff_Create(gReceiveBuffer, RECEIVE_BUFFER_SIZE);  
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void 
USART1_IRQHandler(void)
{
  __HAL_UART_CLEAR_FLAG(&gUart1Handle, UART_FLAG_RXNE);

  volatile uint8_t tempDR = USART1->DR;
  if(CircBuff_Enqueue(&gsReceiveBuffer, tempDR) == 0) 
    {
      Error_Handler();
    }

  const char ack[] = "ACK";
  strTransmit(ack, sizeof(ack));
}

/**
 * @brief   DMA string transmit
 * @note
 * @param   str, size
 * @retval  None
 */
static void 
strTransmit(const char * str, uint8_t size)
{
  /* Check null pointers */
  if(NULL != str)
    {
      /* Wait until DMA2 stream 7 is disabled */
      while(DMA_SxCR_EN == (DMA_SxCR_EN & DMA2_Stream7->CR))
        {
          /* Do nothing, the enable flag shall reset
          * when DMA transfer complete */
        }

      /* Set memory address */
      DMA2_Stream7->M0AR = (uint32_t)str;

      /* Set number of data items */
      DMA2_Stream7->NDTR = size;

      /* Clear all interrupt flags */
      DMA2->HIFCR = (DMA_HIFCR_CFEIF7 | DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CTEIF7
          | DMA_HIFCR_CHTIF7 | DMA_HIFCR_CTCIF7);

      /* Enable DMA2 stream 7 */
      DMA2_Stream7->CR |= DMA_SxCR_EN;
    }
  else
    {
      /* Null pointers, do nothing */
    }
}