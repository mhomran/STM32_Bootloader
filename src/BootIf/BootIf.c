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

#define BOOT_IF_MIN_APP_SECTOR 4
#define BOOT_IF_MAX_APP_SECTOR 11

#define BOOT_IF_IMG_0_SECTOR_0 4
#define BOOT_IF_IMG_0_SECTOR_1 5
#define BOOT_IF_IMG_1_SECTOR_0 6
#define BOOT_IF_IMG_1_SECTOR_1 7
#define BOOT_IF_IMG_2_SECTOR_0 8
#define BOOT_IF_IMG_2_SECTOR_1 9
#define BOOT_IF_IMG_3_SECTOR_0 10
#define BOOT_IF_IMG_3_SECTOR_1 11

#define BOOT_IF_TYPE_DATA_RECORD                          ((PacketType_t)0)
#define BOOT_IF_TYPE_EOF_RECORD                           ((PacketType_t)1)
#define BOOT_IF_TYPE_EXTENDED_LINEAR_ADDR_RECORD          ((PacketType_t)4)

#define ADDR_OFFSET 0
#define TYPE_OFFSET 2
#define DATA_OFFSET 3
#define FRAME_HEADER_SIZE 7 //2 addr, 1 type, 4 CRC
#define FRAME_MAX_DATA_BUFF_SIZE 16
#define FRAME_MAX_SIZE (FRAME_HEADER_SIZE + FRAME_MAX_DATA_BUFF_SIZE)
#define FRAME_CRC_SIZE 4

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "BootIf.h"
#include "PduR.h"
#include "port.h"
#include "error.h"


typedef enum {
  BOOT_IF_TYPE_START = 5,
  BOOT_IF_TYPE_LOCK_FLASH,
  BOOT_IF_TYPE_UNLOCK_FLASH,
  BOOT_IF_TYPE_ERASE_SECTOR,
  BOOT_IF_TYPE_ERASE_IMAGE,
  BOOT_IF_TYPE_RESET,
  BOOT_IF_TYPE_ACK,
  BOOT_IF_TYPE_ERR,
  BOOT_IF_TYPE_END
} PacketType_t;

static uint8_t gTxHexFrameDataBuff[FRAME_MAX_DATA_BUFF_SIZE];
static PduInfo_t gTxPdu;

static Error_t BootIf_Handler(PduInfo_t* pdu);
static StdReturn_t BootIf_EraseSector(uint8_t Sector);
static StdReturn_t BootIf_EraseImage(uint8_t ImgNo);
static StdReturn_t BootIf_TransmitErrorCode(PduId_t id, Error_t err);
static StdReturn_t BootIf_TransmitAck(PduId_t id);

void 
BootIf_Init(void) 
{
  gTxPdu.data = gTxHexFrameDataBuff;
}

/**
 * @brief Handle the received pdu action according to the hex
 * type
 * 
 */
static Error_t 
BootIf_Handler(PduInfo_t* pdu)
{ 
  switch(pdu->data[TYPE_OFFSET])
  {
    case BOOT_IF_TYPE_DATA_RECORD: 
      {

      }
      break;
    case BOOT_IF_TYPE_EOF_RECORD:
      {

      }
      break;
    case BOOT_IF_TYPE_EXTENDED_LINEAR_ADDR_RECORD:
      {

      }
      break;
    case BOOT_IF_TYPE_LOCK_FLASH:
      {
        HAL_FLASH_Lock();
      }
      break;
    case BOOT_IF_TYPE_UNLOCK_FLASH:
      {
        if(HAL_FLASH_Unlock() != HAL_OK)
          {
            return ERR_BOOT_IF_FLASH_UNLOCK;
          }
      }
      break;
    case BOOT_IF_TYPE_ERASE_SECTOR:
      {
        if(BootIf_EraseSector(pdu->data[DATA_OFFSET]) != HAL_OK)
          {
            return ERR_BOOT_IF_SECTOR_ERASE;
          }
      }
      break;
    case BOOT_IF_TYPE_ERASE_IMAGE:
      {
        if(BootIf_EraseImage(pdu->data[DATA_OFFSET]) != HAL_OK)
          {
            return ERR_BOOT_IF_IMAGE_ERASE;
          }
      }
      break;
    case BOOT_IF_TYPE_RESET:
      {
        //Stop the tick, so the wdg isn't fed which leads to reset.
        HAL_SuspendTick();
      }
      break;
    default:
      {
        //DO NOTHING
        return ERR_BOOT_IF_WRONG_TYPE;
      }
      break;
  }

  return ERR_NONE;
}


static StdReturn_t 
BootIf_EraseSector(uint8_t Sector)
{
  FLASH_EraseInitTypeDef herase = {0};
  uint32_t SectorError;

  if(BOOT_IF_MIN_APP_SECTOR <= Sector && Sector <= BOOT_IF_MAX_APP_SECTOR)
    {
      herase.TypeErase = FLASH_TYPEERASE_SECTORS;
      herase.Sector = Sector;
      herase.NbSectors = 1;
      herase.VoltageRange = FLASH_VOLTAGE_RANGE_3;

      if(HAL_FLASHEx_Erase(&herase, &SectorError) != HAL_OK)
        {
          return E_NOT_OK;
        }
      if(SectorError != 0xFFFFFFFFU)
        {
          return E_NOT_OK;
        }
      
      return E_OK;
    }
  else 
    {
      return E_NOT_OK;
    }
}

static StdReturn_t 
BootIf_EraseImage(uint8_t ImgNo)
{
  uint8_t sector_0;
  uint8_t sector_1;

  switch(ImgNo) 
  {
    case 0:
      {
        sector_0 = BOOT_IF_IMG_0_SECTOR_0;
        sector_1 = BOOT_IF_IMG_0_SECTOR_1;
      }
      break;
    case 1:
      {
        sector_0 = BOOT_IF_IMG_1_SECTOR_0;
        sector_1 = BOOT_IF_IMG_1_SECTOR_1;
      }
      break;
    case 2:
      {
        sector_0 = BOOT_IF_IMG_2_SECTOR_0;
        sector_1 = BOOT_IF_IMG_2_SECTOR_1;
      }
      break;
    case 3:
      {
        sector_0 = BOOT_IF_IMG_3_SECTOR_0;
        sector_1 = BOOT_IF_IMG_3_SECTOR_1;
      }
      break;
    default:
      {
        return HAL_ERROR;
      }
      break;
  }
  
  if(BootIf_EraseSector(sector_0) != HAL_OK)
    {
      return HAL_ERROR;
    }
  
  if(BootIf_EraseSector(sector_1) != HAL_OK)
    {
      return HAL_ERROR;
    }

  return HAL_OK;
}

void 
BootIf_RxIndication(PduId_t id, PduInfo_t* pdu)
{
  gErrorCode = BootIf_Handler(pdu);
  if(gErrorCode != ERR_NONE)
    {
      BootIf_TransmitErrorCode(id, gErrorCode);
      return;
    }
  BootIf_TransmitAck(id);
}

static StdReturn_t 
BootIf_TransmitErrorCode(PduId_t id, Error_t err)
{
  gTxPdu.len = FRAME_HEADER_SIZE - FRAME_CRC_SIZE;
  gTxPdu.len += 1; //data size
  *((uint16_t*)&gTxPdu.data[ADDR_OFFSET]) = 0; //address 0
  gTxPdu.data[TYPE_OFFSET] = BOOT_IF_TYPE_ERR; 
  gTxPdu.data[DATA_OFFSET] = err; 

  return PduR_Transmit(id, &gTxPdu);
}

static StdReturn_t 
BootIf_TransmitAck(PduId_t id)
{
  gTxPdu.len = FRAME_HEADER_SIZE - FRAME_CRC_SIZE;
  *((uint16_t*)&gTxPdu.data[ADDR_OFFSET]) = 0; //address 0
  gTxPdu.data[TYPE_OFFSET] = BOOT_IF_TYPE_ACK; 

  return PduR_Transmit(id, &gTxPdu);
}

void 
BootIf_TxConfirmation(PduId_t id) 
{
  
}