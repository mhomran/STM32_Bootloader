/**
 * @file SharedHal.h
 * @author A file comtains a declaration for a struct of function pointers
 *  to STM32CubeHAL APIs.
 * @brief 
 * @version 0.1
 * @date 2021-07-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef SHARED_HAL_H
#define SHARED_HAL_H
/******************************************************************************
* includes
******************************************************************************/
#include "stm32f4xx_hal.h"
/******************************************************************************
* definitions
******************************************************************************/
#define __HAL_GET_HAL_PTR() (const CubeHal_t* const)&_HAL_Flash_Origin
/******************************************************************************
* variables declarations
******************************************************************************/
extern uint32_t _HAL_Flash_Origin;
/******************************************************************************
* typedefs
******************************************************************************/
typedef struct {
//HAL
HAL_StatusTypeDef (*HAL_Init)(void);
void (*HAL_IncTick)(void);
void (*HAL_Delay)(uint32_t Delay);
uint32_t (*HAL_GetTick)(void);
uint32_t (*HAL_GetTickPrio)(void);
HAL_StatusTypeDef (*HAL_SetTickFreq)(HAL_TickFreqTypeDef Freq);
HAL_TickFreqTypeDef (*HAL_GetTickFreq)(void);
void (*HAL_SuspendTick)(void);
void (*HAL_ResumeTick)(void);
void (*HAL_EnableCompensationCell)(void);
void (*HAL_DisableCompensationCell)(void);
//RCC
HAL_StatusTypeDef (*HAL_RCC_DeInit)(void);
HAL_StatusTypeDef (*HAL_RCC_OscConfig)(RCC_OscInitTypeDef *RCC_OscInitStruct);
HAL_StatusTypeDef (*HAL_RCC_ClockConfig)(RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t FLatency);
void     (*HAL_RCC_MCOConfig)(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv);
void     (*HAL_RCC_EnableCSS)(void);
void     (*HAL_RCC_DisableCSS)(void);
uint32_t (*HAL_RCC_GetSysClockFreq)(void);
uint32_t (*HAL_RCC_GetHCLKFreq)(void);
uint32_t (*HAL_RCC_GetPCLK1Freq)(void);
uint32_t (*HAL_RCC_GetPCLK2Freq)(void);
void     (*HAL_RCC_GetOscConfig)(RCC_OscInitTypeDef *RCC_OscInitStruct);
void     (*HAL_RCC_GetClockConfig)(RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t *pFLatency);
HAL_StatusTypeDef (*HAL_RCCEx_PeriphCLKConfig)(RCC_PeriphCLKInitTypeDef  *PeriphClkInit);

//GPIO
void  (*HAL_GPIO_Init)(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);
void  (*HAL_GPIO_DeInit)(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin);
GPIO_PinState (*HAL_GPIO_ReadPin)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void (*HAL_GPIO_WritePin)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void (*HAL_GPIO_TogglePin)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
HAL_StatusTypeDef (*HAL_GPIO_LockPin)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void (*HAL_GPIO_EXTI_IRQHandler)(uint16_t GPIO_Pin);
void (*HAL_GPIO_EXTI_Callback)(uint16_t GPIO_Pin);
//DMA
HAL_StatusTypeDef (*HAL_DMA_Init)(DMA_HandleTypeDef *hdma); 
HAL_StatusTypeDef (*HAL_DMA_DeInit)(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef (*HAL_DMA_Start) (DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef (*HAL_DMA_Start_IT)(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef (*HAL_DMA_Abort)(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef (*HAL_DMA_Abort_IT)(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef (*HAL_DMA_PollForTransfer)(DMA_HandleTypeDef *hdma, HAL_DMA_LevelCompleteTypeDef CompleteLevel, uint32_t Timeout);
void              (*HAL_DMA_IRQHandler)(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef (*HAL_DMA_RegisterCallback)(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID, void (* pCallback)(DMA_HandleTypeDef *_hdma));
HAL_StatusTypeDef (*HAL_DMA_UnRegisterCallback)(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID);
HAL_DMA_StateTypeDef (*HAL_DMA_GetState)(DMA_HandleTypeDef *hdma);
uint32_t             (*HAL_DMA_GetError)(DMA_HandleTypeDef *hdma);
//UART
HAL_StatusTypeDef (*HAL_UART_Init)(UART_HandleTypeDef *huart);
HAL_StatusTypeDef (*HAL_HalfDuplex_Init)(UART_HandleTypeDef *huart);
HAL_StatusTypeDef (*HAL_LIN_Init)(UART_HandleTypeDef *huart, uint32_t BreakDetectLength);
HAL_StatusTypeDef (*HAL_MultiProcessor_Init)(UART_HandleTypeDef *huart, uint8_t Address, uint32_t WakeUpMethod);
HAL_StatusTypeDef (*HAL_UART_DeInit)(UART_HandleTypeDef *huart);
void (*HAL_UART_MspInit)(UART_HandleTypeDef *huart);
void (*HAL_UART_MspDeInit)(UART_HandleTypeDef *huart);
HAL_StatusTypeDef (*HAL_UART_RegisterCallback)(UART_HandleTypeDef *huart, HAL_UART_CallbackIDTypeDef CallbackID, pUART_CallbackTypeDef pCallback);
HAL_StatusTypeDef (*HAL_UART_UnRegisterCallback)(UART_HandleTypeDef *huart, HAL_UART_CallbackIDTypeDef CallbackID);
} CubeHal_t;

#endif
/***************************** END OF FILE ***********************************/