/**
 * @file boot.h
 * @author Mohamed Hassanin
 * @brief This module decides which action to perform on the received packet.
 * @version 0.1
 * @date 2021-07-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#define BOOT_MIN_APP_SECTOR 4
#define BOOT_MAX_APP_SECTOR 11

#define BOOT_IMG_0_SECTOR_0 4
#define BOOT_IMG_0_SECTOR_1 5
#define BOOT_IMG_1_SECTOR_0 6
#define BOOT_IMG_1_SECTOR_1 7
#define BOOT_IMG_2_SECTOR_0 8
#define BOOT_IMG_2_SECTOR_1 9
#define BOOT_IMG_3_SECTOR_0 10
#define BOOT_IMG_3_SECTOR_1 11

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "boot.h"
#include "packet.h"
#include "port.h"
#include "error.h"

static void Boot_Handler(Packet_t*);
static HAL_StatusTypeDef Boot_EraseSector(uint8_t Sector);
static HAL_StatusTypeDef Boot_EraseImage(uint8_t ImgNo);

void 
Boot_Init(void) 
{
  Packet_RegesterBootCallback(Boot_Handler);
}

/**
 * @brief Handle the received packet action according to the packet
 * type
 * 
 */
static void 
Boot_Handler(Packet_t* Packet)
{
  switch(Packet->type)
  {
    case PACKET_TYPE_DATA_RECORD: 
      {

      }
      break;
    case PACKET_TYPE_EOF_RECORD:
      {

      }
      break;
    case PACKET_TYPE_EXTENDED_LINEAR_ADDR_RECORD:
      {

      }
      break;
    case PACKET_TYPE_LOCK_FLASH:
      {
        HAL_FLASH_Lock();
      }
      break;
    case PACKET_TYPE_UNLOCK_FLASH:
      {
        if(HAL_FLASH_Unlock() != HAL_OK)
          {
            gErrorCode = ERR_BOOT_FLASH_UNLOCK;
            Packet_Send(1, 0, PACKET_TYPE_ERR, &gErrorCode);
            return;
          }
      }
      break;
    case PACKET_TYPE_ERASE_SECTOR:
      {
        if(Boot_EraseSector(Packet->data[0]) != HAL_OK)
          {
            gErrorCode = ERR_BOOT_SECTOR_ERASE;
            Packet_Send(1, 0, PACKET_TYPE_ERR, &gErrorCode);
            return;
          }
      }
      break;
    case PACKET_TYPE_ERASE_IMAGE:
      {
        if(Boot_EraseImage(Packet->data[0]) != HAL_OK)
          {
            gErrorCode = ERR_BOOT_IMAGE_ERASE;
            Packet_Send(1, 0, PACKET_TYPE_ERR, &gErrorCode);
            return;
          }
      }
      break;
    case PACKET_TYPE_RESET:
      {
        //Stop the tick, so the wdg isn't fed which leads to reset.
        HAL_SuspendTick();
      }
      break;
    default:
      {
        //DO NOTHING
      }
      break;
  }

  Packet_Send(0, 0, PACKET_TYPE_ACK, 0);
}

static HAL_StatusTypeDef 
Boot_EraseSector(uint8_t Sector)
{
  FLASH_EraseInitTypeDef herase = {0};
  uint32_t SectorError;

  if(BOOT_MIN_APP_SECTOR <= Sector && Sector <= BOOT_MAX_APP_SECTOR)
    {
      herase.TypeErase = FLASH_TYPEERASE_SECTORS;
      herase.Sector = Sector;
      herase.NbSectors = 1;
      herase.VoltageRange = FLASH_VOLTAGE_RANGE_3;

      if(HAL_FLASHEx_Erase(&herase, &SectorError) != HAL_OK)
        {
          gErrorCode = ERR_BOOT_SECTOR_ERASE;
          Packet_Send(1, 0, PACKET_TYPE_ERR, &gErrorCode);
          return HAL_ERROR;
        }
      if(SectorError != 0xFFFFFFFFU)
        {
          gErrorCode = ERR_BOOT_SECTOR_ERASE;
          Packet_Send(1, 0, PACKET_TYPE_ERR, &gErrorCode);
          return HAL_ERROR;
        }
      
      return HAL_OK;
    }
  else 
    {
      return HAL_ERROR;
    }
}

static HAL_StatusTypeDef 
Boot_EraseImage(uint8_t ImgNo)
{
  uint8_t sector_0;
  uint8_t sector_1;

  switch(ImgNo) 
  {
    case 0:
      {
        sector_0 = BOOT_IMG_0_SECTOR_0;
        sector_1 = BOOT_IMG_0_SECTOR_1;
      }
      break;
    case 1:
      {
        sector_0 = BOOT_IMG_1_SECTOR_0;
        sector_1 = BOOT_IMG_1_SECTOR_1;
      }
      break;
    case 2:
      {
        sector_0 = BOOT_IMG_2_SECTOR_0;
        sector_1 = BOOT_IMG_2_SECTOR_1;
      }
      break;
    case 3:
      {
        sector_0 = BOOT_IMG_3_SECTOR_0;
        sector_1 = BOOT_IMG_3_SECTOR_1;
      }
      break;
    default:
      {
        return HAL_ERROR;
      }
      break;
  }
  
  if(Boot_EraseSector(sector_0) != HAL_OK)
    {
      return HAL_ERROR;
    }
  
  if(Boot_EraseSector(sector_1) != HAL_OK)
    {
      return HAL_ERROR;
    }

  return HAL_OK;
}