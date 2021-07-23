#include "main.h"

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  IWDG_HandleTypeDef iwdg_handle = {0};

  iwdg_handle.Instance = IWDG;
  iwdg_handle.Init.Prescaler = IWDG_PRESCALER_4;
  iwdg_handle.Init.Reload = IWDG_2_MS; 
  
  HAL_IWDG_Refresh(&iwdg_handle);
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}