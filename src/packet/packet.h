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

#define PACKET_TYPE_DATA_RECORD                          ((uint8_t)0)
#define PACKET_TYPE_EOF_RECORD                           ((uint8_t)1)
#define PACKET_TYPE_EXTENDED_LINEAR_ADDR_RECORD          ((uint8_t)4)
#define PACKET_TYPE_ERASE_SECTOR_IN_BANK2                ((uint8_t)6)
#define PACKET_TYPE_RESET                                ((uint8_t)7)
#define PACKET_TYPE_ERR                                  ((uint8_t)8)

#include <inttypes.h>

typedef struct {
  uint8_t len;
  uint16_t addr;
  uint8_t* data;
  uint8_t type;
  uint8_t checksum;
} HexPacket_t;
  
#ifdef __cplusplus
extern "C" {
#endif

void Packet_Init(void);
void Packet_Send(uint8_t len, uint16_t address, uint8_t type, uint8_t* data);
void Packet_RegesterBootCallback(void(*pBoot_Handler)(HexPacket_t*));

#ifdef __cplusplus
}
#endif

#endif