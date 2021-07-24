/**
 * @file eeprom_stm32f407vg.h
 * @author Mohamed Hassanin Mohamed
 * @brief Backup SRAM in STM32F407VG can be considered as an 
 * internal EEPROM when VBAT is always present.
 * @version 0.1
 * @date 2021-07-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef EEPROM_STM32F407VG_
#define EEPROM_STM32F407VG_

#ifdef __cplusplus
extern "C" {
#endif

void Eeprom_Init(void);

void Eeprom_WriteByte(uint32_t Address, uint8_t Byte);
uint8_t Eeprom_ReadByte(uint32_t Address);

void Eeprom_WriteHalfWord(uint32_t Address, uint16_t HalfWord);
uint16_t Eeprom_ReadHalfWord(uint32_t Address);

void Eeprom_WriteWord(uint32_t Address, uint32_t Word);
uint32_t Eeprom_ReadWord(uint32_t Address);

#ifdef __cplusplus
}
#endif

#endif