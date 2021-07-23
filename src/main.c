#include "main.h"
#include "port.h"
#include "serial.h"

int 
main(void) 
{ 
  // For systick timer that's used for timeout by CubeHAL
  __enable_irq();
  //interrupts with the same or higher priority are masked
  __set_BASEPRI(TICK_INT_PRIORITY+1); 

  HAL_Init();
  IWDG_Config();
  SystemClock_Config();
  
  Serial_Init();
  BlueLed_Init();

  //interrupts with the same or higher priority are not masked any more
  __set_BASEPRI(0); 

  while (1) 
    {
      HAL_GPIO_TogglePin(GPIOD, BLUE_LED_PIN);
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
BlueLed_Init(void) 
{
  __GPIOD_CLK_ENABLE();
  GPIO_InitTypeDef gpiod_struct = {0};
  gpiod_struct.Mode = GPIO_MODE_OUTPUT_PP;
  gpiod_struct.Pull = GPIO_NOPULL;
  gpiod_struct.Speed = GPIO_SPEED_FREQ_LOW;
  gpiod_struct.Pin = BLUE_LED_PIN;
  HAL_GPIO_Init(GPIOD, &gpiod_struct);
}

void 
IWDG_Config(void) 
{
  IWDG_HandleTypeDef iwdg_handle = {0};

  iwdg_handle.Instance = IWDG;
  iwdg_handle.Init.Prescaler = IWDG_PRESCALER_4;
  iwdg_handle.Init.Reload = IWDG_2_MS; 
  
  if(HAL_IWDG_Init(&iwdg_handle) != HAL_OK)
  {
    Error_Handler();
  }
}


