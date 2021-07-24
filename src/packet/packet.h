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

#include <inttypes.h>
#include "serial.h"

#ifdef __cplusplus
extern "C" {
#endif

void Packet_Init(void);
void Packet_RegesterBootCallback(void(*pBoot_Handler)(HexPacket_t*));

#ifdef __cplusplus
}
#endif

#endif