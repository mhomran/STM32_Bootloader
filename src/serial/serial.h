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

#define UART1_RECEIVE_BUFFER_SIZE (255) 

typedef struct {
  uint8_t len;
  uint16_t addr;
  uint8_t data[UART1_RECEIVE_BUFFER_SIZE];
  uint8_t type;
  uint8_t checksum;
} HexPacket_t;

#ifdef __cplusplus
extern "C" {
#endif

void Serial_Init(void);
void Serial_RegesterPacketCallback(void(*pPacket_Handler)(HexPacket_t*));

#ifdef __cplusplus
}
#endif

#endif