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
/******************************************************************************
* includes
******************************************************************************/
#include "stm32f4xx_hal.h"
#include "eeprom_stm32f407vg.h"
/******************************************************************************
* Functions definitions
******************************************************************************/
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

/**
 * @brief Write a byte to a specific memory location in EEPROM
 * 
 * @param Address 
 * @param Byte 
 */
void 
Eeprom_WriteByte(uint32_t Address, uint8_t Byte)
{
  *(uint8_t*)(BKPSRAM_BASE + Address) = Byte;
}

/**
 * @brief Write a halfword to a specific memory location in EEPROM
 * 
 * @param Address 
 * @param Byte 
 */
void 
Eeprom_WriteHalfWord(uint32_t Address, uint16_t HalfWord)
{
  *(uint16_t*)(BKPSRAM_BASE + Address) = HalfWord;
}

/**
 * @brief Write a word to a specific memory location in EEPROM
 * 
 * @param Address 
 * @param Byte 
 */
void 
Eeprom_WriteWord(uint32_t Address, uint32_t Word)
{
  *(uint32_t*)(BKPSRAM_BASE + Address) = Word;
}

/**
 * @brief Read a byte from a specific memory location in EEPROM
 * 
 * @param Address 
 * @return uint8_t 
 */
uint8_t 
Eeprom_ReadByte(uint32_t Address)
{
  return *(uint8_t*)(BKPSRAM_BASE + Address);
}

/**
 * @brief Read a halfword from a specific memory location in EEPROM
 * 
 * @param Address 
 * @return uint16_t 
 */
uint16_t
Eeprom_ReadHalfWord(uint32_t Address)
{
  return *(uint16_t*)(BKPSRAM_BASE + Address);
}

/**
 * @brief Read a word from a specific memory location in EEPROM
 * 
 * @param Address 
 * @return uint32_t 
 */
uint32_t
Eeprom_ReadWord(uint32_t Address)
{
  return *(uint16_t*)(BKPSRAM_BASE + Address);
}
/***************************** END OF FILE ***********************************/