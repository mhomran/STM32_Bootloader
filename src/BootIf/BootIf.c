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

#define ADDR_OFFSET 0
#define TYPE_OFFSET 2
#define DATA_OFFSET 3
#define FRAME_HEADER_SIZE 7 //2 addr, 1 type, 4 CRC
#define FRAME_MAX_DATA_BUFF_SIZE 16
#define FRAME_MAX_SIZE (FRAME_HEADER_SIZE + FRAME_MAX_DATA_BUFF_SIZE)
#define FRAME_CRC_SIZE 4
#define FRAME_DATA_TO_PROGRAM_SIZE 16

#define DUMMY_ADDR 0

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "BootIf.h"
#include "PduR.h"
#include "port.h"
#include "error.h"
#include "ImgHeader.h"


typedef enum {
  BOOT_IF_TYPE_DATA_RECORD,
  BOOT_IF_TYPE_VERIFY_IMAGE,
  BOOT_IF_TYPE_EXTENDED_LINEAR_ADDR_RECORD,
  BOOT_IF_TYPE_LOCK_FLASH,
  BOOT_IF_TYPE_UNLOCK_FLASH,
  BOOT_IF_TYPE_ERASE_SECTOR,
  BOOT_IF_TYPE_ERASE_IMAGE,
  BOOT_IF_TYPE_RESET,
  BOOT_IF_TYPE_ACK,
  BOOT_IF_TYPE_ERR
} PacketType_t;

static uint8_t gTxHexFrameDataBuff[FRAME_MAX_DATA_BUFF_SIZE];
static PduInfo_t gTxPdu;
extern CRC_HandleTypeDef gCrcHandle;

static Error_t BootIf_Handler(PduInfo_t* pdu);
static StdReturn_t BootIf_EraseSector(uint8_t Sector);
static StdReturn_t BootIf_EraseImage(uint8_t ImgNo);
static StdReturn_t BootIf_VerifyImage(uint8_t ImgNo);
static Error_t BootIf_Flash(PduInfo_t* pdu);
static StdReturn_t BootIf_TransmitErrorCode(PduId_t id, Error_t err);
static StdReturn_t BootIf_TransmitAck(PduId_t id);
static StdReturn_t BootIf_VerifyFlashing(uint8_t* data, uint8_t* start_addr);
static uint32_t AddrSecondHalfWord;

extern uint32_t _App_Flash_Start;
extern uint32_t _App_Flash_End;

extern uint32_t _Image_0_Flash_Size;
extern uint32_t _Image_1_Flash_Size;
extern uint32_t _Image_2_Flash_Size;
extern uint32_t _Image_3_Flash_Size; 

extern uint32_t _Image_0_Flash_Origin;
extern uint32_t _Image_1_Flash_Origin;
extern uint32_t _Image_2_Flash_Origin;
extern uint32_t _Image_3_Flash_Origin;

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
        return BootIf_Flash(pdu);
      }
      break;
    case BOOT_IF_TYPE_VERIFY_IMAGE:
      {
        if(BootIf_VerifyImage(pdu->data[DATA_OFFSET]) != E_OK)
          {
            return ERR_BOOT_IF_IMAGE_VERIFY;
          }
      }
      break;
    case BOOT_IF_TYPE_EXTENDED_LINEAR_ADDR_RECORD:
      {
        AddrSecondHalfWord = *((uint16_t*)&(pdu->data[ADDR_OFFSET]));
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
  *((uint16_t*)&gTxPdu.data[ADDR_OFFSET]) = err; 
  gTxPdu.data[TYPE_OFFSET] = BOOT_IF_TYPE_ERR; 

  return PduR_Transmit(id, &gTxPdu);
}

static StdReturn_t 
BootIf_VerifyFlashing(uint8_t* data, uint8_t* start_addr)
{
  uint8_t read;
  uint8_t sent;
  for(uint8_t i = 0; i < FRAME_DATA_TO_PROGRAM_SIZE; i++)
    {
      read = *(i+start_addr);
      sent = data[i];
      if(read != sent)
        {
          return E_NOT_OK;
        }
    }
  return E_OK;
}

static StdReturn_t 
BootIf_TransmitAck(PduId_t id)
{
  gTxPdu.len = FRAME_HEADER_SIZE - FRAME_CRC_SIZE;
  *((uint16_t*)&gTxPdu.data[ADDR_OFFSET]) = DUMMY_ADDR; 
  gTxPdu.data[TYPE_OFFSET] = BOOT_IF_TYPE_ACK; 

  return PduR_Transmit(id, &gTxPdu);
}


static Error_t 
BootIf_Flash(PduInfo_t* pdu)
{
  uint32_t AddrFirstHalfWord;
  uint32_t PgAddr;
  uint32_t DataWord;
  uint8_t i;
  uint8_t DataLen;

  DataLen = pdu->len - (FRAME_HEADER_SIZE - FRAME_CRC_SIZE);
  if(DataLen != FRAME_DATA_TO_PROGRAM_SIZE)
    {
      return ERR_BOOT_IF_PROGRAM_FORMAT;
    }

  AddrFirstHalfWord = (uint32_t)(*((uint16_t*)&pdu->data[ADDR_OFFSET]));
  PgAddr = (AddrSecondHalfWord << 16) | AddrFirstHalfWord;

  //to not cross the 128-bit row boundary causing alignment error
  if((PgAddr % 4) != 0)
    {
      return ERR_BOOT_IF_PROGRAM_ADDR_ALIGN;
    }
  
  if((uint32_t)&_App_Flash_End < PgAddr || PgAddr < (uint32_t)&_App_Flash_Start)
    {
      return ERR_BOOT_IF_ADDR_OUTBOUND;
    }

  for(i = 0; i < FRAME_DATA_TO_PROGRAM_SIZE; i = i + 4)
    {
      DataWord = *((uint32_t*)&pdu->data[DATA_OFFSET + i]);
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, PgAddr + i, DataWord);
    }
  
  if(E_NOT_OK == BootIf_VerifyFlashing(&(pdu->data[DATA_OFFSET]), (uint8_t*)PgAddr))
    {
      return ERR_BOOT_IF_DATA_IS_NOT_WRITTEN;
    }

  return ERR_NONE;
}

static StdReturn_t 
BootIf_VerifyImage(uint8_t ImgNo)
{
  uint32_t image_size;
  uint32_t image_origin;
  ImgHeader_t* image_header;
  uint32_t addr;
  uint32_t crc;

  switch(ImgNo) 
  {
    case 0:
      {
        image_size = (uint32_t)&_Image_0_Flash_Size;
        image_origin = (uint32_t)& _Image_0_Flash_Origin;
      }
      break;
    case 1:
      {
        image_size = (uint32_t)&_Image_1_Flash_Size;
        image_origin = (uint32_t)& _Image_1_Flash_Origin;
      }
      break;
    case 2:
      {
        image_size = (uint32_t)&_Image_2_Flash_Size;
        image_origin = (uint32_t)& _Image_2_Flash_Origin;
      }
      break;
    case 3:
      {
        image_size = (uint32_t)&_Image_3_Flash_Size;
        image_origin = (uint32_t)& _Image_3_Flash_Origin;
      }
      break;
    default:
      {
        return E_NOT_OK;
      }
      break;
  }

  image_header = (ImgHeader_t *)image_origin;

  if((image_header->ImageEndAddr - image_header->ImageStartAddr) > image_size)
    {
      return E_NOT_OK;
    }
  
  if(image_header->EntryPointAddr == 0xFFFFFFFF)
    {
      return E_NOT_OK;
    }

  __HAL_CRC_DR_RESET(&gCrcHandle);
  addr = image_header->ImageStartAddr + sizeof(ImgHeader_t);
  while(addr < image_header->ImageEndAddr)
    {
      uint8_t data = *((uint8_t*)addr);
      uint32_t data32 = (uint32_t)data;
      gCrcHandle.Instance->DR = data32;
      addr += 1;
    }
  crc = gCrcHandle.Instance->DR;

  if(crc != image_header->CRC32)
    {
      return E_NOT_OK;
    }
  
  return E_OK;
}

void 
BootIf_TxConfirmation(PduId_t id) 
{
  
}