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

#define USART1_RECEIVE_BUFFER_SIZE 100

#ifdef __cplusplus
extern "C" {
#endif

void Serial_Init(void);

#ifdef __cplusplus
}
#endif

#endif