#ifndef MAIN_H
#define MAIN_H

#define IWDG_2_MS 16

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void SystemClock_Config(void);
void IWDG_Config(void);
void BlueLed_Init(void);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif