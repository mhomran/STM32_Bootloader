/**
 * @file packet.h
 * @author Mohamed Hassanin Mohamed
 * @brief A packet handler for HEX packets, it checks the checksum and
 * call the bootloader to determine the action.
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef PACKET_H
#define PACKET_H

#define PACKET_TYPE_DATA_RECORD                          ((PacketType_t)0)
#define PACKET_TYPE_EOF_RECORD                           ((PacketType_t)1)
#define PACKET_TYPE_EXTENDED_LINEAR_ADDR_RECORD          ((PacketType_t)4)

#include <inttypes.h>

typedef enum {
  PACKET_TYPE_START = 5,
  PACKET_TYPE_LOCK_FLASH,
  PACKET_TYPE_UNLOCK_FLASH,
  PACKET_TYPE_ERASE_SECTOR,
  PACKET_TYPE_ERASE_IMAGE,
  PACKET_TYPE_RESET,
  PACKET_TYPE_ACK,
  PACKET_TYPE_ERR,
  PACKET_TYPE_END
} PacketType_t;

typedef struct {
  uint8_t len;
  uint16_t addr;
  uint8_t* data;
  uint8_t type;
  uint8_t checksum;
} Packet_t;

#ifdef __cplusplus
extern "C" {
#endif

void Packet_Init(void);
void Packet_Send(uint8_t len, uint16_t address, uint8_t type, uint8_t* data);
void Packet_RegesterBootCallback(void(*pBoot_Handler)(Packet_t*));

#ifdef __cplusplus
}
#endif

#endif