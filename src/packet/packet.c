
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

static const char gChecksumError[] = "[ERROR] Checksum";

static void Packet_Handler(HexPacket_t* Packet);
inline static uint8_t Packet_Checksum(HexPacket_t* Packet);

void 
Packet_Init(void)
{
  Serial_RegesterPacketCallback(Packet_Handler);
}

static void 
Packet_Handler(HexPacket_t* Packet)
{
  if(Packet_Checksum(Packet) == 0)
    {
      printf("%s\n", gChecksumError);
      return;
    }
}

inline static uint8_t 
Packet_Checksum(HexPacket_t* Packet)
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

  return (sum == Packet->checksum);
}