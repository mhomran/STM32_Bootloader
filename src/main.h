/**
 * @file main.h
 * @author Mohamed Hassanin Mohamed
 * @brief A bootloader for STM32F407VG MCU
 * @version 0.1
 * @date 2021-07-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef MAIN_H
#define MAIN_H
/******************************************************************************
* includes
******************************************************************************/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "port.h"
#include "Uart.h"
#include "UartIf.h"
#include "PduR.h"
#include "BootIf.h"
#include "eeprom_stm32f407vg.h"
/******************************************************************************
* Definitions
******************************************************************************/
#define IWDG_4_S 0xFFF
/******************************************************************************
* Functions prototypes
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void SystemClock_Config(void);
void IWDG_Config(void);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
/***************************** END OF FILE ***********************************/