/**
 * @file serial.h
 * @author Mohamed Hassanin
 * @brief A serial communication used by the bootloader (USART at this case)
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef SERIAL_H
#define SERIAL_H

#include <inttypes.h>
#include "packet.h"

#ifdef __cplusplus
extern "C" {
#endif

void Serial_Init(void);
void Serial_Send(HexPacket_t*);
void Serial_RegesterPacketCallback(void(*pPacket_Handler)(HexPacket_t*));

#ifdef __cplusplus
}
#endif

#endif