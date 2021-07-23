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

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "serial.h"
#include "port.h"
#include "circ_buffer.h"

void Error_Handler(void);

static UART_HandleTypeDef gUart1Handle;
static DMA_HandleTypeDef gDma2Handle;
static uint8_t gReceiveBuffer[USART1_RECEIVE_BUFFER_SIZE];
static CircBuff_t gsReceiveBuffer;

static const char gOvfError[] = "[ERROR] USART1 circular buffer overflow";

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

      __HAL_LINKDMA(&gUart1Handle,hdmatx,gDma2Handle);

      /* USART1 interrupt Init */
      HAL_NVIC_SetPriority(USART1_IRQn, TICK_INT_PRIORITY+1, 0);
      HAL_NVIC_EnableIRQ(USART1_IRQn);
      HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

      /* Enable the UART Data Register not empty Interrupt */
      __HAL_UART_ENABLE_IT(&gUart1Handle, UART_IT_RXNE);
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
  
  gsReceiveBuffer = CircBuff_Create(gReceiveBuffer, USART1_RECEIVE_BUFFER_SIZE);  
}


/**
  * @brief This function handles USART1 global interrupt.
  */
void 
USART1_IRQHandler(void)
{
  volatile uint8_t RCFlag = __HAL_UART_GET_FLAG(&gUart1Handle, UART_FLAG_RXNE);
  //clear the RC flag to prevent stm32 HAL from clearing it and disable the receiver
  if(RCFlag) 
    {
      __HAL_UART_CLEAR_FLAG(&gUart1Handle, UART_FLAG_RXNE);
    }
  
  HAL_UART_IRQHandler(&gUart1Handle);
    
  if(RCFlag) 
    {
      volatile uint8_t tempDR = USART1->DR;
      if(CircBuff_Enqueue(&gsReceiveBuffer, tempDR) == 0) 
        {
          printf("%s\n", gOvfError);
        }
    }
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&gDma2Handle);
}

int 
_write(int file, char *ptr, int len) 
{  
  return HAL_UART_Transmit_DMA(&gUart1Handle, (uint8_t*)ptr, len);
}
