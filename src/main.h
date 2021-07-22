#ifndef MAIN_H
#define MAIN_H

#define RED_LED_PIN GPIO_PIN_15

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void SystemClock_Config(void);
void RedLed_Init(void);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif