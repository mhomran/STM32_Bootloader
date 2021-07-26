/**
 * @file uart_if.h
 * @author Mohamed Hassanin
 * @brief UART interface layer between the driver and the PDU router.
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef UART_IF_H
#define UART_IF_H

#include <inttypes.h>
#include "UartIf.h"
#include "Uart.h"
#include "PduR.h"

#ifdef __cplusplus
extern "C" {
#endif

void UartIf_Init(void);
StdReturn_t UartIf_Transmit(PduId_t, PduInfo_t*);
void UartIf_RxIndication(UartHw_t Uart, PduInfo_t* Pdu);
void UartIf_TxConfirmation(PduId_t);

#ifdef __cplusplus
}
#endif

#endif