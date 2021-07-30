/**
 * @file Uart.h
 * @author Mohamed Hassanin
 * @brief A Uart driver to pack the hex formated packet and send it to
 * UartIf.
 * @version 0.1
 * @date 2021-07-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef UART_H
#define UART_H

/******************************************************************************
* Includes
******************************************************************************/
#include <inttypes.h>
#include "Std_Types.h"
#include "PduR.h"
/******************************************************************************
* delarations
******************************************************************************/
typedef struct {
  uint8_t len;
  uint8_t* data;
  PduId_t pduid;
} UartPdu_t;

typedef enum {
  UART_HW_1,
  UART_HW_MAX
} UartHw_t;

/******************************************************************************
* Function Prototypes
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void Uart_Init(void);
StdReturn_t Uart_Write(UartHw_t, UartPdu_t*);

#ifdef __cplusplus
}
#endif

#endif
/***************************** END OF FILE ***********************************/