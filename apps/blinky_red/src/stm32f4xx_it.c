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
* variables defintions
******************************************************************************/
extern const CubeHal_t* const pHal;
/******************************************************************************
* Functions defintions
******************************************************************************/
/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  pHal->HAL_IncTick();
}

void EXTI0_IRQHandler(void)
{
  pHal->HAL_IncTick();
}
/***************************** END OF FILE ***********************************/