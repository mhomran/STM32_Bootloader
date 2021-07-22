#include "main.h"

int 
main(void) 
{
  __enable_irq(); // For systick timer that's used for timeout by CubeHAL
  HAL_Init();
  SystemClock_Config();

  RedLed_Init();
  
  while (1) 
    {
      HAL_GPIO_TogglePin(GPIOD, RED_LED_PIN);
      HAL_Delay(500);
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void 
SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
      Error_Handler();
    }

  SystemCoreClockUpdate();
}

void 
Error_Handler(void) 
{

}

void 
RedLed_Init(void) 
{
  __GPIOD_CLK_ENABLE();
  GPIO_InitTypeDef gpiod_struct = {0};
  gpiod_struct.Mode = GPIO_MODE_OUTPUT_PP;
  gpiod_struct.Pull = GPIO_NOPULL;
  gpiod_struct.Speed = GPIO_SPEED_FREQ_LOW;
  gpiod_struct.Pin = RED_LED_PIN;
  HAL_GPIO_Init(GPIOD, &gpiod_struct);
}


