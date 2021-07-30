/**
 * @file stm32f4xx_it.c
 * @author Mohamed Hassanin
 * @brief This file contains all the used interrupt handlers
 * @version 0.1
 * @date 2021-07-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/******************************************************************************
* Includes
******************************************************************************/
#include "main.h"
/******************************************************************************
* Variable declarations
******************************************************************************/
extern UART_HandleTypeDef gUart1Handle;
extern DMA_HandleTypeDef gDma2Usart1TxHandle;
extern DMA_HandleTypeDef gDma2Usart1RxHandle;
/******************************************************************************
* Functions prototypes
******************************************************************************/
void UART1_IDLE_IRQHandler(UART_HandleTypeDef *huart);
/******************************************************************************
* Functions defintions
******************************************************************************/
/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  IWDG_HandleTypeDef iwdg_handle = {0};

  iwdg_handle.Instance = IWDG;
  iwdg_handle.Init.Prescaler = IWDG_PRESCALER_4;
  iwdg_handle.Init.Reload = IWDG_4_S; 
  
  HAL_IWDG_Refresh(&iwdg_handle);
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}


/**
  * @brief This function handles USART1 global interrupt.
  */
void 
USART1_IRQHandler(void)
{ 
  if(__HAL_UART_GET_FLAG(&gUart1Handle, UART_FLAG_IDLE))
    {
      __HAL_UART_CLEAR_FLAG(&gUart1Handle, UART_FLAG_IDLE);
      HAL_UART_AbortReceive(&gUart1Handle);
      UART1_IDLE_IRQHandler(&gUart1Handle);
    }
  
  //clears the TX and RX flags
  HAL_UART_IRQHandler(&gUart1Handle);
}


/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&gDma2Usart1TxHandle);
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&gDma2Usart1RxHandle);
}
/***************************** END OF FILE ***********************************/