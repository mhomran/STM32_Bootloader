
/**
 * @file packet.c
 * @author Mohamed Hassanin Mohamed
 * @brief A packet handler for HEX packets, it checks the checksum and
 * call the bootloader to determine the action.
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include "packet.h"
#include "serial.h"
#include "port.h"
#include "error.h"

static void Packet_Handler(Packet_t* Packet);
inline static uint8_t Packet_Checksum(Packet_t* Packet);
inline static uint8_t Packet_IsCorrectType(uint8_t);
static void (*Boot_Handler)(Packet_t*);

static Packet_t gPacket;

void 
Packet_Init(void)
{
  Serial_RegesterPacketCallback(Packet_Handler);
}

static void 
Packet_Handler(Packet_t* Packet)
{
  if(Packet_Checksum(Packet) != Packet->checksum)
    {
      gErrorCode = ERR_PACKET_CHECKSUM;
      Packet_Send(1, 0, PACKET_TYPE_ERR, &gErrorCode);
      return;
    }
  
  if(Packet_IsCorrectType(Packet->type) == 0)
    {
      gErrorCode = ERR_PACKET_WRONG_TYPE;
      Packet_Send(1, 0, PACKET_TYPE_ERR, &gErrorCode);
      return;
    }
  
  switch(Packet->type)
  {
    case PACKET_TYPE_ERASE_SECTOR:
      {
        if(Packet->len != 1)
          {
            gErrorCode = ERR_BOOT_SECTOR_ERASE;
            Packet_Send(1, 0, PACKET_TYPE_ERR, &gErrorCode);
            return;
          }
      }
      break;
    case PACKET_TYPE_ERASE_IMAGE:
      {
        if(Packet->len != 1)
          {
            gErrorCode = ERR_BOOT_IMAGE_ERASE;
            Packet_Send(1, 0, PACKET_TYPE_ERR, &gErrorCode);
            return;
          }
      }
      break;
    default:
      {
        //UNREACHABLE
      }
      break;
  }
  
  Boot_Handler(Packet);
}

inline static uint8_t 
Packet_Checksum(Packet_t* Packet)
{
  uint8_t sum = 0;
  sum += Packet->len;
  sum += (uint8_t)(Packet->addr & 0x00FF);
  sum += (uint8_t)((Packet->addr & 0xFF00) >> 8);
  sum += Packet->type;

  for(uint8_t i = 0; i < Packet->len; i++) 
    {
      sum += Packet->data[i];
    }
  
  sum = ~sum;
  sum += 1;

  return sum;
}

void 
Packet_RegesterBootCallback(void(*pBoot_Handler)(Packet_t*))
{
  Boot_Handler = pBoot_Handler;
}

void 
Packet_Send(uint8_t len, uint16_t address, uint8_t type, uint8_t* data)
{
  gPacket.len = len;
  gPacket.addr = address;
  gPacket.type = type;
  gPacket.data = data;
  gPacket.checksum = Packet_Checksum(&gPacket);
  Serial_Send(&gPacket);
}

inline static uint8_t 
Packet_IsCorrectType(uint8_t Type)
{
  if(
    Type != PACKET_TYPE_DATA_RECORD &&
    Type != PACKET_TYPE_EOF_RECORD &&
    Type != PACKET_TYPE_EXTENDED_LINEAR_ADDR_RECORD &&
    Type >= PACKET_TYPE_END &&
    Type <= PACKET_TYPE_START
    )
    {
      return 0;
    }

  return 1;
}