/**
 * @file eeprom_stm32f407vg.c
 * @author Mohamed Hassanin Mohamed
 * @brief Backup SRAM in STM32F407VG can be considered as an 
 * internal EEPROM when VBAT is always present.
 * @version 0.1
 * @date 2021-07-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "stm32f4xx_hal.h"
#include "eeprom_stm32f407vg.h"

/**
 * @brief Initialize the EEPROM module.
 * 
 */
void 
Eeprom_Init(void)
{
  __PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_BKPSRAM_CLK_ENABLE();
}

void 
Eeprom_WriteByte(uint32_t Address, uint8_t Byte)
{
  *(uint8_t*)(BKPSRAM_BASE + Address) = Byte;
}

void 
Eeprom_WriteHalfWord(uint32_t Address, uint16_t HalfWord)
{
  *(uint16_t*)(BKPSRAM_BASE + Address) = HalfWord;
}

void 
Eeprom_WriteWord(uint32_t Address, uint32_t Word)
{
  *(uint32_t*)(BKPSRAM_BASE + Address) = Word;
}

uint8_t 
Eeprom_ReadByte(uint32_t Address)
{
  return *(uint8_t*)(BKPSRAM_BASE + Address);
}

uint16_t
Eeprom_ReadHalfWord(uint32_t Address)
{
  return *(uint16_t*)(BKPSRAM_BASE + Address);
}

uint32_t
Eeprom_ReadWord(uint32_t Address)
{
  return *(uint16_t*)(BKPSRAM_BASE + Address);
}
