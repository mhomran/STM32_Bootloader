/**
 * @file SharedHal.c
 * @author A file comtains a definition for a struct of function pointers
 *  to STM32CubeHAL APIs.
 * @brief 
 * @version 0.1
 * @date 2021-07-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/******************************************************************************
* includes
******************************************************************************/
#include "SharedHal.h"
/******************************************************************************
* variables definitions
******************************************************************************/
static volatile const CubeHal_t CubeHal __attribute__((section(".hal"))) = {
//HAL
HAL_Init,
HAL_IncTick,
HAL_Delay,
HAL_GetTick,
HAL_GetTickPrio,
HAL_SetTickFreq,
HAL_GetTickFreq,
HAL_SuspendTick,
HAL_ResumeTick,
HAL_EnableCompensationCell,
HAL_DisableCompensationCell,
//RCC
HAL_RCC_DeInit,
HAL_RCC_OscConfig,
HAL_RCC_ClockConfig,
HAL_RCC_MCOConfig,
HAL_RCC_EnableCSS,
HAL_RCC_DisableCSS,
HAL_RCC_GetSysClockFreq,
HAL_RCC_GetHCLKFreq,
HAL_RCC_GetPCLK1Freq,
HAL_RCC_GetPCLK2Freq,
HAL_RCC_GetOscConfig,
HAL_RCC_GetClockConfig,
HAL_RCCEx_PeriphCLKConfig,
//GPIO
HAL_GPIO_Init,
HAL_GPIO_DeInit,
HAL_GPIO_ReadPin,
HAL_GPIO_WritePin,
HAL_GPIO_TogglePin,
HAL_GPIO_LockPin,
HAL_GPIO_EXTI_IRQHandler,
HAL_GPIO_EXTI_Callback,
//DMA
HAL_DMA_Init,
HAL_DMA_DeInit,
HAL_DMA_Start,
HAL_DMA_Start_IT,
HAL_DMA_Abort,
HAL_DMA_Abort_IT,
HAL_DMA_PollForTransfer,
HAL_DMA_IRQHandler,
HAL_DMA_RegisterCallback,
HAL_DMA_UnRegisterCallback,
HAL_DMA_GetState,
HAL_DMA_GetError,
//UART
HAL_UART_Init,
HAL_HalfDuplex_Init,
HAL_LIN_Init,
HAL_MultiProcessor_Init,
HAL_UART_DeInit,
HAL_UART_MspInit,
HAL_UART_MspDeInit,
HAL_UART_RegisterCallback,
HAL_UART_UnRegisterCallback
};
/***************************** END OF FILE ***********************************/
