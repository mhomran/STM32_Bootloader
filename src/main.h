#ifndef MAIN_H
#define MAIN_H

#define IWDG_4_S 0xFFF

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "port.h"
#include "Uart.h"
#include "UartIf.h"
#include "PduR.h"
#include "BootIf.h"
#include "eeprom_stm32f407vg.h"

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